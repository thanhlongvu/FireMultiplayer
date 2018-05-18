#pragma once
#include "AssetManager.h"

class Bullet
{
public:
	Bullet(float x, float y);
	Bullet(sf::Vector2f);
	Bullet();
	~Bullet();

	sf::Sprite& GetBulletSprite() const;

	void Move(sf::Vector2f dir, float deltaTime);

	sf::Vector2f GetPosition() const;

	float GetTimeExist() const;

	bool isExist() const;

	void SetBulletDirection(const sf::Vector2f dir);

	sf::Vector2f GetBulletDirection() const;

private:
	float speed;
	float damage;
	float timeExist;


	sf::Vector2f direction;

	sf::Clock timer;

private:
	sf::Sprite* bulletSprite;
};

