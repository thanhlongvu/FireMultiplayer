#pragma once
#include <SFML/Graphics.hpp>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	static AssetManager* assets();

	sf::Texture* GetPlayerTexture() const;
	sf::Texture* GetBulletTexture() const;


private:
	//Bullet
	sf::Texture* bulletTexture;

	//Player
	sf::Texture* playerTexture;

private:
	static AssetManager* m_assets;

};

