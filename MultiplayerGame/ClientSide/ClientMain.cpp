#include "Client.h"
#include <SFML\Graphics.hpp>
#include "Game.h"

void HandlePacket(const PacketID& l_id, sf::Packet& l_packet, Client* l_client)
{
	if ((PacketType)l_id == PacketType::Message)
	{
		std::string message;
		l_packet >> message;

		std::cout << message << std::endl;
	}
	else if ((PacketType)l_id == PacketType::Disconnect)
	{
		//TODO: When Server send disconnect then it will close, due to client to do disconnect method will fail
		l_client->Disconnect();
		
	}

	else if ((PacketType)l_id == PacketType::Player_Disconnect)
	{
		int id_player;
		l_packet >> id_player;
		Game::playerManager->RemovePlayer(id_player);
	}

	else if ((PacketType)l_id == PacketType::List_Player)
	{
		int id_player, heart_player;
		sf::Vector2f pos_player;
		l_packet >> id_player >> heart_player >> pos_player.x >> pos_player.y;

		//Update to playerManager
		Player* player = new Player();
		player->setHeart(heart_player);
		player->setPosition(pos_player);

		Game::playerManager->players[id_player] = player;
	}

	//Move packet
	else if ((PacketType)l_id == PacketType::Player_Move)
	{
		int id_player;
		float x, y;
		l_packet >> id_player >> x >> y;

		//Update
		Game::playerManager->GetPlayer(id_player).setPosition(x, y);
	}

	//Fire packet
	else if ((PacketType)l_id == PacketType::Player_Fire)
	{
		int id_player;
		float x, y;

		sf::Vector2f dir;

		l_packet >> id_player >> x >> y >> dir.x >> dir.y;

		Bullet* b = new Bullet(x, y);
		b->SetBulletDirection(dir);

		Game::bullets.push_back(b);
	}
	//else if()...
}


void CommandProcess(Client* l_client)
{
	while (l_client->isConnected())
	{
		std::string str;
		std::getline(std::cin, str);

		if (str != "")
		{
			if (str == "/quit")
			{
				if (l_client->Disconnect())
					break;
				else
					continue;
			}
			else if (str == "/clear")
			{
				system("cls");
				continue;
			}

			//Message to Server
			sf::Packet p;
			StampPacket(PacketType::Message, p);

			p << str;
			l_client->Send(p);
		}
	}
}

void GameWindow(Client* l_client)
{
	Game game(l_client);
	while (!game.GetWindow()->IsDone()) {
		// Game loop.
		
		//Handle input and send to server
		game.HandleInput();
		game.Update();
		game.Broadcast(l_client);
		game.Render();
		game.RestartClock();
	}

}



int main()
{
	//Ip and port of server
	std::string ipAddress;
	PortNumber port = (unsigned short)Network::ServerPort;

	std::cout << "Enter server ip: " << std::endl;
	std::cin >> ipAddress;


	sf::IpAddress ip(ipAddress);


	Client client;
	client.SetServerInformation(ip, port);
	client.Setup(&HandlePacket);

	sf::Thread c(&CommandProcess, &client);
	sf::Thread w(&GameWindow, &client);

	if (client.Connect())
	{
		c.launch();
		w.launch();

		sf::Clock clock;

		clock.restart();
		while (client.isConnected())
		{
			client.Update(clock.restart());
		}
	}
	else
	{
		std::cout << "Fail to connect to Server..." << std::endl;
	}

	std::cout << "Quit..." << std::endl;
	
	
	sf::sleep(sf::seconds(2.f));
	return 0;
}