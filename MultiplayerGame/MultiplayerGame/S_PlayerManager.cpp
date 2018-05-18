#include "S_PlayerManager.h"



S_PlayerManager::S_PlayerManager()
{
}


S_PlayerManager::~S_PlayerManager()
{
}

void S_PlayerManager::AddPlayer(const int id_player)
{
	Player* player = new Player();

	//Random spawn position
	int x = rand() % 250;
	int y = rand() % 250;
	player->setPosition(x, y);

	m_players.insert(std::make_pair(id_player, player));
}

void S_PlayerManager::RemovePlayer(const int id_player)
{
	m_players.erase(id_player);
}

Player* S_PlayerManager::GetPlayer(const int id) const
{
	return m_players.at(id);
}

int S_PlayerManager::GetNumberPlayer() const
{
	return m_players.size();
}
