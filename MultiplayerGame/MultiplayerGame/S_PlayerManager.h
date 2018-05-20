#pragma once
#include <unordered_map>
#include "NetworkDefinitions.h"
#include "Player.h"
#include "S_BulletManager.h"

class S_PlayerManager
{
public:
	S_PlayerManager();
	~S_PlayerManager();

	void AddPlayer(const int, const std::string& name, const int& teamIndex);
	void RemovePlayer(const int);

	Player* GetPlayer(const int) const;

	void CheckCollider(S_BulletManager& bullets);

	int GetNumberPlayer() const;



public:
	std::unordered_map<ClientID, Player*> m_players;

private:
	float CalculatorDistance(const sf::Vector2f&, const sf::Vector2f&);
};

