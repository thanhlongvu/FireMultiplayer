#pragma once
#include "WindowGame.h"
#include "Player.h"
#include <queue>
#include <vector>
#include <SFML/Network.hpp>
#include "PacketTypes.h"
#include "Client.h"
#include "C_PlayerManager.h"
#include "Bullet.h"

class Game {
public:
	Game(Client* client);
	~Game();

	void HandleInput();
	void Update();
	void Render();

	void Broadcast(Client* l_client);

	WindowGame* GetWindow();

	sf::Time GetElapsed();
	void RestartClock();

	C_PlayerManager* GetPlayerManager() const;
	
public:
	std::queue<sf::Packet*> listPacket;
	static C_PlayerManager* playerManager;

	static std::vector<Bullet*> bullets;

private:

	WindowGame m_window;
	sf::Clock m_clock;
	sf::Time m_elapsed;

	Player m_player;

	sf::Vector2f prePosition;

	bool isUpdate = false;

	//Bullet
	sf::Clock cooldownBullet;


private:
	float distance2Point(float x, float y, float a, float b);
};