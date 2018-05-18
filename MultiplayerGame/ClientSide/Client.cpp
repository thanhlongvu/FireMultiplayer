#include "Client.h"


//bind the listening thread to the Listen method of this class
Client::Client() :m_listenThread(&Client::Listen, this)
{
}


Client::~Client()
{
	m_socket.unbind();
}

bool Client::Connect()
{
	if (m_connected)
		return false;

	m_socket.bind(sf::Socket::AnyPort);

	sf::Packet p;
	StampPacket(PacketType::Connect, p);
	p << m_playerName;

	//Check send status
	if (m_socket.send(p, m_serverIp, m_serverPort) != sf::Socket::Done)
	{
		m_socket.unbind();
		return false;
	}

	m_socket.setBlocking(false);
	p.clear();

	sf::IpAddress recvIP;
	PortNumber recvPORT;
	sf::Clock timer;
	timer.restart();


	//Check timeout
	while (timer.getElapsedTime().asMilliseconds() < CONNECT_TIMEOUT)
	{
		std::cout << ".";
		sf::Socket::Status s = m_socket.receive(p, recvIP, recvPORT);

		//due to m_socket is non-blocking mode
		if (s != sf::Socket::Done)
			continue;
		//check ip address
		if (recvIP != m_serverIp)
		{
			continue;
		}

		PacketID id;

		if (!(p >> id >> id_client))
		{
			std::cout << "Packet send to invalid" << std::endl;
			break;
		}
		if ((PacketType)id != PacketType::Connect)
		{
			std::cout << "Packet ID invalid" << std::endl;
			continue;
		}

		std::cout << std::endl;
		std::cout << "Connect to Server SUCCESS with id: " << id_client << std::endl;

		//Handler packet
		m_packetHandler(id, p, this);
		m_connected = true;
		m_socket.setBlocking(true);
		m_lastHeartbeat = m_serverTime;
		m_listenThread.launch();

		return true;
	}

	std::cout << std::endl;

	//Connect is failed
	std::cout << "Connect attempt failed! Server info: " << m_serverIp << ":" << m_serverPort << std::endl;
	m_socket.unbind();
	m_socket.setBlocking(true);

	return false;
}

bool Client::Disconnect()
{

	if (!m_connected)
		return false;

	sf::Packet p;
	StampPacket(PacketType::Disconnect, p);

	sf::Socket::Status status = m_socket.send(p, m_serverIp, m_serverPort);
	if (status != sf::Socket::Done)
	{
		return false;
	}

	//Terminal Listen thread
	m_listenThread.terminate();

	m_socket.unbind();
	m_connected = false;

	std::cout << "Send Quit packet success" << std::endl;
	return true;
}

void Client::Listen()
{
	sf::Packet packet;
	sf::IpAddress recvIP;
	PortNumber recvPORT;

	while (m_connected)
	{
		packet.clear();
		sf::Socket::Status status = m_socket.receive(packet, recvIP, recvPORT);

		if (status != sf::Socket::Done)
		{
			if (m_connected)
			{
				std::cout << "Failed receiving a packet from Server." << std::endl;
				continue;
			}
			else {
				std::cout << "Socket unbound." << std::endl;
				break;
			}
		}

		if (recvIP != m_serverIp)
		{
			continue;
		}

		PacketID p_id;
		if (!(packet >> p_id))
			continue;

		PacketType id = (PacketType)p_id;

		//Invalid packet type
		if (id < PacketType::Disconnect || id >= PacketType::OutOfBounds)
			continue;

		// time synchronization and maintaining a valid connection
		if (id == PacketType::Heartbeat)
		{
			sf::Packet p;
			StampPacket(PacketType::Heartbeat, p);
			if (m_socket.send(p, m_serverIp, m_serverPort) != sf::Socket::Done)
			{
				std::cout << "Failed to sending a heartbeat!" << std::endl;
			}

			sf::Int32 timestamp;
			packet >> timestamp;
			SetTime(sf::milliseconds(timestamp));
			m_lastHeartbeat = m_serverTime;
		}
		else if (m_packetHandler)
		{
			m_packetHandler((PacketID)id, packet, this);
		}

	}
}

bool Client::Send(sf::Packet & l_packet)
{
	if (!m_connected) return false;

	if (m_socket.send(l_packet, m_serverIp, m_serverPort) != sf::Socket::Done)
	{
		std::cout << "Failed when send a packet" << std::endl;
		return false;
	}
		
	return true;
}

const sf::Time & Client::GetTime() const
{
	return m_serverTime;
}

const sf::Time & Client::GetLastHeartbeat() const
{
	return m_lastHeartbeat;
}

void Client::SetTime(const sf::Time & l_time)
{
	m_serverTime = l_time;
}

void Client::SetServerInformation(const sf::IpAddress & l_ip, const PortNumber & l_port)
{
	m_serverIp = l_ip;
	m_serverPort = l_port;
}

void Client::Setup(void(*l_handler)(const PacketID &, sf::Packet &, Client *))
{
	m_packetHandler = std::bind(l_handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void Client::UnregisterPacketHandler()
{
	m_packetHandler = nullptr;
}

void Client::Update(const sf::Time & l_time)
{
	if (!m_connected) return;

	m_serverTime += l_time;

	if (m_serverTime.asMilliseconds() < 0)
	{
		m_serverTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
		m_lastHeartbeat = m_serverTime;
		return;
	}

	if (m_serverTime.asMilliseconds() - m_lastHeartbeat.asMilliseconds() >= sf::Int32(Network::ClientTimeout))
	{
		//Timeout
		std::cout << "Server connection time out!" << std::endl;
		Disconnect();
	}
}

bool Client::isConnected() const
{
	return m_connected;;
}

void Client::SetPlayerName(const std::string & l_name)
{
	m_playerName = l_name;
}

sf::Mutex & Client::GetMutex()
{
	return m_mutex;
}

int Client::GetClientID() const
{
	return id_client;
}

