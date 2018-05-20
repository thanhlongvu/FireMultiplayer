#include "Bullet.h"


Bullet::Bullet(float x, float y, const int& bulletOfTeam) : Bullet()
{
	m_bulletOfTeam = bulletOfTeam;

	bulletSprite = new sf::Sprite(*AssetManager::assets()->GetBulletTexture(bulletOfTeam));
	bulletSprite->setScale(0.2f, 0.2f);
	bulletSprite->setPosition(x, y);
}

Bullet::Bullet(sf::Vector2f pos, const int& bulletOfTeam) : Bullet()
{
	m_bulletOfTeam = bulletOfTeam;

	bulletSprite = new sf::Sprite(*AssetManager::assets()->GetBulletTexture(bulletOfTeam));
	bulletSprite->setScale(0.2f, 0.2f);
	bulletSprite->setPosition(pos);
}

Bullet::Bullet()
{
	damage = 10;
	speed = 1000;
	timeExist = 1.5f;

	timer.restart();

}


Bullet::~Bullet()
{
	delete(bulletSprite);
}

sf::Sprite& Bullet::GetBulletSprite() const
{
	return *bulletSprite;
}


//Using deltaTime to calculator S = V * delta T
void Bullet::Move(sf::Vector2f dir, float deltaTime)
{
	bulletSprite->setPosition(bulletSprite->getPosition() + dir * deltaTime);
}


sf::Vector2f Bullet::GetPosition() const
{
	return bulletSprite->getPosition();
}

float Bullet::GetTimeExist() const
{
	return timeExist;
}

bool Bullet::isExist() const
{
	if (timer.getElapsedTime().asSeconds() >= timeExist)
		return false;

	return true;
}

void Bullet::SetBulletDirection(const sf::Vector2f dir)
{
	this->direction = dir;

	float lengthDir = sqrt(pow(dir.x, 2) + pow(dir.y, 2));
	
	float xDir, yDir;
	xDir = dir.x / lengthDir;
	yDir = dir.y / lengthDir;

	this->direction = sf::Vector2f(xDir, yDir);
}

sf::Vector2f Bullet::GetBulletDirection() const
{
	return this->direction;
}

int Bullet::GetBulletOfTeam() const
{
	return m_bulletOfTeam;
}

float Bullet::GetSpeed() const
{
	return speed;
}
