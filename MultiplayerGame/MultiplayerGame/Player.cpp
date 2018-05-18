#include "Player.h"



Player::Player() : m_heart(100)
{
	m_playerTexture = new sf::Texture();
	m_playerSprite = new sf::Sprite();
	this->setTexture("E:\\VisualStudio\\SFML\\MultiplayerGame\\ClientSide\\Player.png");
	this->setSprite();	

}


Player::~Player()
{
}

sf::Vector2f Player::getPosition() const
{
	return m_position;
}

void Player::setPosition(sf::Vector2f pos)
{
	
	m_position = pos;
	m_playerSprite->setPosition(pos);
}

void Player::setPosition(float x, float y)
{
	sf::Vector2f pos(x, y);
	m_position = pos;

	m_playerSprite->setPosition(pos);
}

int Player::getHeart() const
{
	return m_heart;
}

void Player::setHeart(const int heart)
{
	m_heart = heart;
}

void Player::setTexture(std::string texturePath)
{
	m_playerTexture->loadFromFile(texturePath);
}

void Player::setSprite()
{
	m_playerSprite->setTexture(*m_playerTexture);
}

sf::Sprite& Player::getSprite() const
{
	return *m_playerSprite;
}
