#include "Server.h"



Server::Server(void(*l_handler)(sf::IpAddress &, const PortNumber &, const PacketID &, sf::Packet &, Server *)) : m_listenThread(&Server::Listen, this)
{
	m_packetHandler = std::bind(l_handler, std::placeholders::_1, std::placeholders::_2,
		std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);


}

Server::~Server()
{
	Stop();
}

void Server::BindTimeoutHandler(void(*l_handler)(const ClientID &))
{
	m_timeoutHandler = std::bind(l_handler, std::placeholders::_1);
}

bool Server::Send(const ClientID & l_id, sf::Packet & l_packet)
{
	sf::Lock lock(m_mutex);
	//Find client with id in client list
	auto itr = m_clients.find(l_id);

	//if don't see client
	if (itr == m_clients.end())
		return false;

	//Send to the client
	if (m_outgoing.send(l_packet, itr->second.m_clientIP, itr->second.m_clientPORT) != sf::Socket::Done)
	{
		std::cout << "Error when send packet..." << std::endl;
		return false;
	}

	m_totalSent += l_packet.getDataSize();
	return true;
}

bool Server::Send(sf::IpAddress & l_ip, const PortNumber & l_port, sf::Packet & l_packet)
{
	if (m_outgoing.send(l_packet, l_ip, l_port) != sf::Socket::Done)
		return false;

	m_totalSent += l_packet.getDataSize();

	return true;
}

void Server::Broadcast(sf::Packet & l_packet, const ClientID & l_ignore)
{
	sf::Lock lock(m_mutex);

	for (auto &itr : m_clients)
	{
		if (itr.first != l_ignore)
		{
			if (!Send(itr.first, l_packet))
			{
				std::cout << "Error when send broadcasting message to client: " << itr.first << std::endl;
				continue;
			}
		}
	}
}

void Server::Listen()
{
	sf::IpAddress ip;
	PortNumber port;
	sf::Packet packet;
	while (m_running)
	{
		packet.clear();
		sf::Socket::Status status = m_incoming.receive(packet, ip, port);
		if (status != sf::Socket::Done)
		{
			if (m_running)
			{
				std::cout << "Error receive packet from: " << ip << ":" << port << ". Status: " << status << std::endl;
				continue;
			}
			else
			{
				std::cout << "Socket unbound" << std::endl;
				break;
			}
		}

		m_totalreceived += packet.getDataSize();

		PacketID p_id;
		if (!(packet >> p_id)) continue;

		PacketType id = (PacketType)p_id;
		//Check valid ID
		if (id < PacketType::Connect || id > PacketType::OutOfBounds)
			continue;

		if (id == PacketType::Heartbeat)
		{
			sf::Lock lock(m_mutex);

			for (auto &itr : m_clients)
			{
				if (itr.second.m_clientIP != ip || itr.second.m_clientPORT != port)
					continue;

				//m_heartbeatWaiting is true when server request 
				if (!itr.second.m_heartbeatWaiting)
				{
					std::cout << "Packet received invalid" << std::endl;
					break;
				}


				itr.second.m_ping = m_serverTime.asMilliseconds() - itr.second.m_heartbeatSent.asMilliseconds();
				itr.second.m_lastHeartbeat = m_serverTime;
				itr.second.m_heartbeatWaiting = false;

				itr.second.m_heartbeatRetry = 0;
				break;
			}
		}
		else if (m_packetHandler)
		{
			m_packetHandler(ip, port, (PacketID)id, packet, this);
		}

	}
}

//UDP protocol is connectionless
//due to should check status connect by heartbeat
void Server::Update(const sf::Time & l_time)
{
	m_serverTime += l_time;

	//Over number 
	if (m_serverTime.asMilliseconds() < 0)
	{
		m_serverTime -= sf::milliseconds((sf::Int32)Network::HighestTimestamp);

		sf::Lock lock(m_mutex);

		for (auto &itr : m_clients)
		{
			itr.second.m_lastHeartbeat = sf::milliseconds(std::abs(itr.second.m_lastHeartbeat.asMilliseconds()
				- (sf::Int32)Network::HighestTimestamp));
		}
	}


	sf::Lock lock(m_mutex);
	for (auto itr = m_clients.begin(); itr != m_clients.end();)
	{
		sf::Int32 elapsed = m_serverTime.asMilliseconds() - itr->second.m_lastHeartbeat.asMilliseconds();

		if (elapsed >= HEARTBEAT_INTERVAL)
		{
			//Non reply, timeout
			if (elapsed >= (sf::Int32) Network::ClientTimeout || itr->second.m_heartbeatRetry > HEARTBEAT_RETRIES)
				//Remove client
			{

				std::cout << "Client " << itr->first << " has time out! Elapsed: " << elapsed << std::endl;
				if (m_timeoutHandler)
				{
					m_timeoutHandler(itr->first);
				}
				
				
				//Remove player
				playerManager.RemovePlayer(itr->first);
				
				//Delete this player to other clients
				sf::Packet p;
				StampPacket(PacketType::Player_Disconnect, p);
				p << itr->first;

				Broadcast(p);

				itr = m_clients.erase(itr);

				continue;
			}

			//Create a Heartbeat 
			if (!itr->second.m_heartbeatWaiting || (elapsed >= HEARTBEAT_INTERVAL * (itr->second.m_heartbeatRetry + 1)))
			{
				//Heartbeat
				if (itr->second.m_heartbeatRetry >= 1)
				{
					std::cout << "Retry(" << itr->second.m_heartbeatRetry << ") heartbeat for client " << itr->first << std::endl;
				}

				sf::Packet Heartbeat;
				StampPacket(PacketType::Heartbeat, Heartbeat);

				Heartbeat << m_serverTime.asMilliseconds();
				Send(itr->first, Heartbeat);

				if (itr->second.m_heartbeatRetry == 0)
				{
					itr->second.m_heartbeatSent = m_serverTime;
				}

				itr->second.m_heartbeatWaiting = true;
				++itr->second.m_heartbeatRetry;

				m_totalSent += Heartbeat.getDataSize();
			}
		}
		++itr;
	}
}

ClientID Server::AddClient(const sf::IpAddress & l_ip, const PortNumber & l_port)
{
	sf::Lock lock(m_mutex);

	for (auto &itr : m_clients)
	{
		if (itr.second.m_clientIP == l_ip && itr.second.m_clientPORT == l_port)
			return ClientID(Network::NullID);
	}

	ClientID id = m_lastID;
	ClientInfo info(l_ip, l_port, m_serverTime);
	m_clients.insert(std::make_pair(id, info));

	//Add Player
	playerManager.AddPlayer(id);

	++m_lastID;

	return id;
}

ClientID Server::GetClientID(const sf::IpAddress & l_ip, const PortNumber & l_port)
{
	sf::Lock lock(m_mutex);
	for (auto &itr : m_clients)
	{
		if (itr.second.m_clientIP == l_ip && itr.second.m_clientPORT == l_port)
			return itr.first;
	}

	return ClientID(Network::NullID);
}

bool Server::HasClient(const ClientID & l_id)
{
	return (m_clients.find(l_id) != m_clients.end());
}

bool Server::HasClient(const sf::IpAddress & l_ip, const PortNumber & l_port)
{
	return (GetClientID(l_ip, l_port) >= 0);
}

bool Server::GetClientInfo(const ClientID & l_id, ClientInfo & l_info)
{
	sf::Lock lock(m_mutex);

	for (auto itr : m_clients)
	{
		if (itr.first == l_id)
		{
			l_info = itr.second;
			return true;
		}
	}

	return false;
}

bool Server::RemoveClient(const ClientID & l_id)
{
	sf::Lock lock(m_mutex);
	auto itr = m_clients.find(l_id);
	if (itr == m_clients.end())
		return false;

	/*sf::Packet p;
	StampPacket(PacketType::Disconnect, p);
	Send(l_id, p);*/

	m_clients.erase(itr);

	//Remove player
	playerManager.RemovePlayer(l_id);

	return true;
}

bool Server::RemoveClient(const sf::IpAddress & l_ip, const PortNumber & l_port)
{
	sf::Lock lock(m_mutex);
	for (auto itr = m_clients.begin(); itr != m_clients.end(); itr++)
	{
		if (itr->second.m_clientIP == l_ip && itr->second.m_clientPORT == l_port)
		{
			sf::Packet p;
			StampPacket(PacketType::Disconnect, p);
			Send(itr->first, p);

			m_clients.erase(itr);

			//Remove player
			playerManager.RemovePlayer(itr->first);

			return true;
		}
	}

	return false;
}

void Server::DisconnectAll()
{
	if (!m_running) return;
	sf::Packet p;
	StampPacket(PacketType::Disconnect, p);

	Broadcast(p);

	sf::Lock lock(m_mutex);
	m_clients.clear();
}

bool Server::Start()
{
	if (m_running)
		return false;

	if (m_incoming.bind((unsigned short)Network::ServerPort) != sf::Socket::Done)
	{
		return false;
	}

	m_outgoing.bind(sf::Socket::AnyPort);

	Setup();

	std::cout << "Incoming port: " << m_incoming.getLocalPort() << ". Outgoing port: " << m_outgoing.getLocalPort() << std::endl;
	m_running = true;

	m_listenThread.launch();

	return true;
}

bool Server::Stop()
{
	if (!m_running) return false;

	DisconnectAll();
	m_running = false;
	m_incoming.unbind();//Stop thread listen

	return true;

}

bool Server::IsRunning()
{
	return m_running;
}

unsigned int Server::GetClientCount()
{
	return (unsigned int)m_clients.size();
}

std::string Server::GetClientList()
{
	std::string list;
	std::string delimiter = "--------------------------------------";
	list = delimiter;
	list += '\n';
	list += "ID";
	list += '\t';
	list += "Client IP:PORT";
	list += '\t';
	list += '\t';
	list += "Ping";
	list += '\n';
	list += delimiter;
	list += '\n';
	for (auto itr = m_clients.begin(); itr != m_clients.end(); ++itr) {
		list += std::to_string(itr->first);
		list += '\t';
		list += itr->second.m_clientIP.toString() + ":" + std::to_string(itr->second.m_clientPORT);
		list += '\t';
		list += '\t';
		list += std::to_string(itr->second.m_ping) + "ms.";
		list += '\n';
	}
	list += delimiter;
	list += '\n';
	list += "Total data sent: " + std::to_string(m_totalSent / 1000) + "kB. Total data received: " + std::to_string(m_totalreceived / 1000) + "kB";
	return list;
}

sf::Mutex & Server::GetMutex()
{
	return m_mutex;
}

sf::Time Server::GetTimeServer() const
{
	return m_serverTime;
}

void Server::Setup()
{
	m_lastID = 0;
	m_running = false;
	m_totalSent = 0;
	m_totalreceived = 0;
}
