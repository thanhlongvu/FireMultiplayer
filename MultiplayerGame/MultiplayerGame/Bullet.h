#pragma once
#include "AssetManager.h"

class Bullet
{
public:
	Bullet(float x, float y, const int& bulletOfTeam);
	Bullet(sf::Vector2f, const int& bulletOfTeam);
	Bullet();
	~Bullet();

	sf::Sprite& GetBulletSprite() const;

	void Move(sf::Vector2f dir, float deltaTime);


	float GetTimeExist() const;

	bool isExist() const;

	void SetBulletDirection(const sf::Vector2f dir);

	sf::Vector2f GetBulletDirection() const;

	int GetBulletOfTeam() const;

	int GetIndexAtMap() const;

	void SetIndexAtMap(int index);

	void SetPosition(const sf::Vector2f&);

	void SetPosition(float x, float y);

	sf::Vector2f GetPosition() const;

	float GetDamage() const;

	float GetSpeed() const;


public:
	int m_bulletOfTeam;
	

private:
	float speed;
	float damage;
	float timeExist;


	sf::Vector2f direction;
	sf::Vector2f position;

	sf::Clock timer;

	int m_indexAtMap;

private:
	sf::Sprite* bulletSprite;
};

