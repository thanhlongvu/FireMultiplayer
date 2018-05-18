#pragma once
#include <unordered_map>
#include "NetworkDefinitions.h"
#include "Player.h"

class S_PlayerManager
{
public:
	S_PlayerManager();
	~S_PlayerManager();

	void AddPlayer(const int);
	void RemovePlayer(const int);

	Player* GetPlayer(const int) const;

	int GetNumberPlayer() const;


public:
	std::unordered_map<ClientID, Player*> m_players;
};

