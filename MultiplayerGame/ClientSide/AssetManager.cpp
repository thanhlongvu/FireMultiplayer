#include "AssetManager.h"

AssetManager* AssetManager::m_assets = NULL;

AssetManager::AssetManager()
{
	bulletTexture = new sf::Texture();
	bulletTexture->loadFromFile("E:\\VisualStudio\\SFML\\MultiplayerGame\\ClientSide\\Bullet.png");

	playerTexture = new sf::Texture();
	playerTexture->loadFromFile("E:\\VisualStudio\\SFML\\MultiplayerGame\\ClientSide\\Player.png");
	playerTexture->setSmooth(true);
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

sf::Texture * AssetManager::GetPlayerTexture() const
{
	return playerTexture;
}

sf::Texture * AssetManager::GetBulletTexture() const
{
	return bulletTexture;
}

