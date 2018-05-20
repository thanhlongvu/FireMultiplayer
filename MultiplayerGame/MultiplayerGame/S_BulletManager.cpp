#include "S_BulletManager.h"
#include <algorithm>



S_BulletManager::S_BulletManager() : count_BulletTeamOne(0), count_BulletTeamTwo(0)
{
}


S_BulletManager::~S_BulletManager()
{
}

void S_BulletManager::AddBullet(Bullet * b)
{
	if (b->GetBulletOfTeam() == 1)
	{
		b->SetIndexAtMap(count_BulletTeamOne);
		bulletsTeamOne.insert(std::make_pair(count_BulletTeamOne, b));

		count_BulletTeamOne++;
	}
	else
	{
		b->SetIndexAtMap(count_BulletTeamTwo);
		bulletsTeamTwo.insert(std::make_pair(count_BulletTeamTwo, b));

		count_BulletTeamTwo++;
	}
}

void S_BulletManager::RemoveBullet(Bullet * b)
{
	//Find index of bullet
	if (b->GetBulletOfTeam() == 1)
	{
		int index = b->GetIndexAtMap();

		bulletsTeamOne.erase(index);
	
	}
	else
	{
		int index = b->GetIndexAtMap();

		bulletsTeamTwo.erase(index);
	}
}

void S_BulletManager::UpdateBullets(const sf::Time& l_time)
{
	if (bulletsTeamOne.size())
		for (auto& itr : bulletsTeamOne)
		{

			Bullet* b = itr.second;

			if (!b->isExist())
			{
				bulletsTeamOne.erase(itr.first);
				return;
			}


			b->Move(b->GetBulletDirection(), b->GetSpeed() * l_time.asSeconds());
		}

	if (bulletsTeamTwo.size())
		for (auto& itr : bulletsTeamTwo)
		{

			Bullet* b = itr.second;

			if (!b->isExist())
			{
				bulletsTeamTwo.erase(itr.first);
				return;
			}


			b->Move(b->GetBulletDirection(), 500 * l_time.asSeconds());
		}
}
