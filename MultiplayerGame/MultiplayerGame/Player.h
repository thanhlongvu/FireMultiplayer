#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string.h>
#include "AssetManager.h"

class Player
{
public:
	Player(int, std::string);
	~Player();

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f pos);
	void setPosition(float x, float y);

	int getHeart() const;
	void setHeart(const int heart);

	void setTexture(std::string texturePath);

	void setSprite();

	sf::Sprite& getSprite() const;

	int GetTeamIndex() const;

	void SetTeamIndex(int teamIndex);

	std::string GetPlayerName() const;

	void SetPlayerName(const std::string);

	bool isDie() const;

private:
	int m_heart;

	sf::Texture* m_playerTexture;
	sf::Sprite* m_playerSprite;
	sf::Vector2f m_position;

	//Team index
	int m_teamIndex;
	std::string m_name;
};

