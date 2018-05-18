#include "C_PlayerManager.h"



C_PlayerManager::C_PlayerManager()
{
}


C_PlayerManager::~C_PlayerManager()
{
}

//Player* C_PlayerManager::GetMainPlayer() const
//{
//	return players.begin;
//}

void C_PlayerManager::AddPlayer(const int id_player)
{
	Player* player = new Player();
	players.insert(std::make_pair(id_player, player));
}

void C_PlayerManager::RemovePlayer(const int id_player)
{
	players.erase(id_player);
}

Player C_PlayerManager::GetPlayer(const int id) const
{
	return *(players.at(id));
}
