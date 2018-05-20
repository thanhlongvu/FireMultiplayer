#include "Player.h"


Player::Player(int teamIndex, std::string name) : m_heart(100), m_speed(0.5f), m_teamIndex(teamIndex), m_name(name)
{
	m_playerTexture = AssetManager::assets()->GetPlayerTexture(teamIndex);
	m_playerSprite = new sf::Sprite(*m_playerTexture);
	m_playerSprite->setScale(0.5f, 0.5f);
	m_playerSprite->setOrigin(m_playerTexture->getSize().x / 2, m_playerTexture->getSize().y / 2);

}

Player::Player() : m_heart(100), m_speed(0.5f), m_teamIndex(1), m_name("No Name")
{
	m_playerTexture = AssetManager::assets()->GetPlayerTexture(m_teamIndex);
	m_playerSprite = new sf::Sprite(*m_playerTexture);
	m_playerSprite->setScale(0.5f, 0.5f);
	m_playerSprite->setOrigin(m_playerTexture->getSize().x / 2, m_playerTexture->getSize().y / 2);
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

int Player::GetTeamIndex() const
{
	return m_teamIndex;
}

void Player::SetTeamIndex(int teamIndex)
{
	m_teamIndex = teamIndex;
}

std::string Player::GetPlayerName() const
{
	return m_name;
}

void Player::SetPlayerName(const std::string name)
{
	m_name = name;
}

float Player::GetSpeed() const
{
	return m_speed;
}
