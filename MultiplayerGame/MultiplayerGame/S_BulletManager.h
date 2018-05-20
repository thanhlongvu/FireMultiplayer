#pragma once
#include <unordered_map>
#include <vector>
#include "NetworkDefinitions.h"
#include "Bullet.h"


class S_BulletManager
{
public:
	S_BulletManager();
	~S_BulletManager();

	void AddBullet(Bullet*);
	void RemoveBullet(Bullet*);

	void UpdateBullets(const sf::Time& l_time);

public:
	/*std::vector<Bullet*> bulletsTeamOne;
	std::vector<Bullet*> bulletsTeamTwo;*/

	std::unordered_map<int32_t, Bullet*> bulletsTeamOne;
	std::unordered_map<int32_t, Bullet*> bulletsTeamTwo;

private:
	int32_t count_BulletTeamOne;
	int32_t count_BulletTeamTwo;
};

