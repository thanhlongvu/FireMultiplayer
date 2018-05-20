#include "Bullet.h"
#include <iostream>

Bullet::Bullet(float x, float y, const int& bulletOfTeam) : Bullet()
{
	m_bulletOfTeam = bulletOfTeam;

	bulletSprite = new sf::Sprite(*AssetManager::assets()->GetBulletTexture(bulletOfTeam));
	bulletSprite->setScale(0.2f, 0.2f);
	this->SetPosition(x, y);
}

Bullet::Bullet(sf::Vector2f pos, const int& bulletOfTeam) : Bullet()
{
	m_bulletOfTeam = bulletOfTeam;

	bulletSprite = new sf::Sprite(*AssetManager::assets()->GetBulletTexture(bulletOfTeam));
	bulletSprite->setScale(0.2f, 0.2f);
	this->SetPosition(pos);
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
	position += dir * deltaTime;
	bulletSprite->setPosition(position);
}


sf::Vector2f Bullet::GetPosition() const
{
	return position;
}

float Bullet::GetDamage() const
{
	return damage;
}

void Bullet::SetPosition(const sf::Vector2f & pos)
{
	position = pos;

	bulletSprite->setPosition(position);
}

void Bullet::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;

	bulletSprite->setPosition(position);
}

float Bullet::GetTimeExist() const
{
	return timeExist;
}

bool Bullet::isExist() const
{
	//std::cout << timer.getElapsedTime().asSeconds() << std::endl;
	if (timer.getElapsedTime().asSeconds() >= timeExist)
		return false;

	return true;
}

void Bullet::SetBulletDirection(const sf::Vector2f dir)
{
	//this->direction = dir;

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

int Bullet::GetIndexAtMap() const
{
	return m_indexAtMap;
}

void Bullet::SetIndexAtMap(int index)
{
	m_indexAtMap = index;
}

float Bullet::GetSpeed() const
{
	return speed;
}

