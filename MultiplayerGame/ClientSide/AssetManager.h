#pragma once
#include <SFML/Graphics.hpp>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	static AssetManager* assets();

	sf::Texture* GetPlayerTexture(const int&) const;
	sf::Texture* GetBulletTexture(const int&) const;
	sf::Texture* GetCircleTexture() const;

private:
	//Bullet
	sf::Texture* bulletTexture;
	sf::Texture* bulletTexture2;

	//Player
	sf::Texture* playerTexture;
	sf::Texture* playerTexture2;

	//Circle
	sf::Texture* circleTexture;
private:
	static AssetManager* m_assets;

};

