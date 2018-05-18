#include "Server.h"
#include "EntityEvent.h"

void Handler(sf::IpAddress& l_ip, const PortNumber& l_port, const PacketID& l_id, sf::Packet& l_packet, Server* l_server)
{
	ClientID id = l_server->GetClientID(l_ip, l_port);

	if (id >= 0)
	{
		if ((PacketType)l_id == PacketType::Disconnect)
		{
			l_server->RemoveClient(id);

			//Notification
			std::cout << "Client " << id << " quit!" << std::endl;

			//Send notification to all the other client
			sf::Packet p;
			StampPacket(PacketType::Message, p);

			std::string message = "Client left! " + l_ip.toString() + ":" + std::to_string(l_port);

			p << message;

			l_server->Broadcast(p, id);

			//Delete player

			p.clear();
			StampPacket(PacketType::Player_Disconnect, p);
			p << id;

			l_server->Broadcast(p);
		}

		else if ((PacketType)l_id == PacketType::Message)
		{
			std::string receivedMessage;
			l_packet >> receivedMessage;

			//String show to client
			std::string message = l_ip.toString() + ":" + std::to_string(l_port) + " :" + receivedMessage;

			std::cout << message << std::endl;

			sf::Packet p;
			StampPacket(PacketType::Message, p);

			p << message;

			l_server->Broadcast(p, id);
		}

		//Move packet
		else if ((PacketType)l_id == PacketType::Player_Move)
		{
			float x, y;
			l_packet >> x >> y;

			//Set to playerManager
			l_server->playerManager.GetPlayer(id)->setPosition(x, y);

			//Send message to the other players
			sf::Packet p;
			CreateMovePacket(p, id, x, y);

			for (auto itr = l_server->playerManager.m_players.begin(); itr != l_server->playerManager.m_players.end(); itr++)
			{
				if (itr->first != id)
				{
					l_server->Send(itr->first, p);
				}
			}
		}

		//Fire packet
		else if ((PacketType)l_id == PacketType::Player_Fire)
		{
			//Example
			sf::Vector2f pos;
			sf::Vector2f dir;
			l_packet >> pos.x >> pos.y >> dir.x >> dir.y;

			sf::Packet p;
			CreateFirePacket(p, id, pos, dir);
			for (auto itr = l_server->playerManager.m_players.begin(); itr != l_server->playerManager.m_players.end(); itr++)
			{
				if (itr->first != id)
				{
					l_server->Send(itr->first, p);
				}
			}
		}
	}
	else
	{
		if ((PacketType)l_id == PacketType::Connect)
		{
			std::cout << l_ip << ":" << l_port << " request connect to..." << std::endl;

			ClientID id = l_server->AddClient(l_ip, l_port);

			sf::Packet packet;

			StampPacket(PacketType::Connect, packet);

			//Add id of the client
			packet << id;

			l_server->Send(id, packet);

			//Send information other players to the new player
			for (auto itr = l_server->playerManager.m_players.begin(); itr != l_server->playerManager.m_players.end(); itr++)
			{
				packet.clear();

				StampPacket(PacketType::List_Player, packet);

				PlayerStatus(itr->first, itr->second, packet);

				l_server->Send(id, packet);
			}

			//Send information of new player to other players
			packet.clear();
			StampPacket(PacketType::List_Player, packet);
			PlayerStatus(id, l_server->playerManager.GetPlayer(id), packet);
			for (auto itr = l_server->playerManager.m_players.begin(); itr != l_server->playerManager.m_players.end(); itr++)
			{
				if (itr->first != id)
				{
					l_server->Send(itr->first, packet);
				}
			}

		}
	}
}


void CommandProcess(Server* l_server)
{
	while (l_server->IsRunning())
	{
		std::string str;
		std::getline(std::cin, str);

		if (str == "/quit")
		{
			l_server->Stop();
			break;
		}
		else if (str == "/dc")
		{
			l_server->DisconnectAll();
			std::cout << "Disconnect all " << std::endl;
		}
		else if (str == "/list")
		{
			std::cout << l_server->GetClientCount() << " clients online:" << std::endl;
			std::cout << l_server->GetClientList() << std::endl;

		}
		else if (str == "/time")
		{

			std::cout << l_server->GetTimeServer().asMilliseconds() << std::endl;
		}
		else if (str == "/clear")
		{
			system("cls");
		}
	}
}



int main()
{
	Server server(Handler);

	if (server.Start())
	{
		//Command process thread
		sf::Thread c(&CommandProcess, &server);
		c.launch();

		sf::Clock clock;
		clock.restart();

		while (server.IsRunning())
		{
			server.Update(clock.restart());
		}
		std::cout << "Stopping server..." << std::endl;
	}

	system("pause");
	return 0;
}