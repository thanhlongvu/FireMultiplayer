#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string.h>

class Player
{
public:
	Player();
	~Player();

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f pos);
	void setPosition(float x, float y);

	int getHeart() const;
	void setHeart(const int heart);

	void setTexture(std::string texturePath);

	void setSprite();
	sf::Sprite& getSprite() const;

private:
	int m_heart;
	sf::Texture* m_playerTexture;
	sf::Sprite* m_playerSprite;
	sf::Vector2f m_position;
};

