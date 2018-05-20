#include "AssetManager.h"

AssetManager* AssetManager::m_assets = NULL;

AssetManager::AssetManager()
{
	bulletTexture = new sf::Texture();
	bulletTexture->loadFromFile("Assets\\Bullet.png");

	bulletTexture2 = new sf::Texture();
	bulletTexture2->loadFromFile("Assets\\Bullet2.png");

	playerTexture = new sf::Texture();
	playerTexture->loadFromFile("Assets\\Player.png");
	playerTexture->setSmooth(true);

	playerTexture2 = new sf::Texture();
	playerTexture2->loadFromFile("Assets\\Player2.png");
	playerTexture2->setSmooth(true);
}


AssetManager::~AssetManager()
{
}

AssetManager * AssetManager::assets()
{
	if (m_assets == NULL)
	{
		m_assets = new AssetManager();
	}
	return m_assets;
}

sf::Texture * AssetManager::GetPlayerTexture(const int& index) const
{
	if(index == 1)
		return playerTexture;

	return playerTexture2;
}

sf::Texture * AssetManager::GetBulletTexture(const int& index) const
{
	if(index == 1)
		return bulletTexture;

	return bulletTexture2;
}

