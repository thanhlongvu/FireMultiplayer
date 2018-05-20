#include "S_PlayerManager.h"
#include <iostream>
#include "PacketTypes.h"


S_PlayerManager::S_PlayerManager()
{
}


S_PlayerManager::~S_PlayerManager()
{
}

void S_PlayerManager::AddPlayer(const int id_player, const std::string& name, const int& teamIndex)
{
	Player* player = new Player(teamIndex, name);

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

void S_PlayerManager::CheckCollider(S_BulletManager& bullets)
{
	if (!m_players.size())
		return;

	for (auto& itr : m_players)
	{
		Player* player = itr.second;

		auto bulletTeam = player->GetTeamIndex() == 1 ? bullets.bulletsTeamTwo : bullets.bulletsTeamOne;

		//Check BulletTeamTwo
		if (!bulletTeam.size())
			continue;
		for (auto& bullet : bulletTeam)
		{
			Bullet* b = bullet.second;

			float dis = CalculatorDistance(player->getPosition(), b->GetPosition());

			if (dis < 100.f)
			//Collider
			{
				//Decrease heart of player
				player->setHeart(player->getHeart() - b->GetDamage());

				//Destroy this bullet
				bulletTeam.erase(bullet.first);

				//Create a packet send to players
				//by id_player
				sf::Packet p;
				CreateHurtPacket(p, itr.first, b->GetPosition());

				//Send broadcast
				//l_server->Broadcast(p);
				
			}
		}
	}
}

int S_PlayerManager::GetNumberPlayer() const
{
	return m_players.size();
}


float S_PlayerManager::CalculatorDistance(const sf::Vector2f & a, const sf::Vector2f & b)
{
	float dis = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	return dis;
}
