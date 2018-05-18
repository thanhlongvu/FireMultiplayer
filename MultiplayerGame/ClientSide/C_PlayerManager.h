#pragma once
#include <unordered_map>
#include "Player.h"

class C_PlayerManager
{
public:
	C_PlayerManager();
	~C_PlayerManager();

	//Player* GetMainPlayer() const;
	void AddPlayer(const int);
	void RemovePlayer(const int);

	Player GetPlayer(const int id) const;


public:
	//Player main_player;
	std::unordered_map<int, Player*> players;
};

