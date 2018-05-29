#include "Game.h"
#include <iostream>

C_PlayerManager* Game::playerManager = new C_PlayerManager();
std::vector<Bullet*> Game::bullets = std::vector<Bullet*>();

bool Game::isPlaying = true;

Game::Game(Client* client) : m_window("Window", sf::Vector2u(1000, 700)) 
{
	RestartClock();
	srand(time(NULL));

	// Setting up class members.
	//Position of player

	
	m_player = *playerManager->GetPlayer(client->GetClientID());
	//m_player.SetCircleSprite();

	prePosition = m_player.getPosition();
	
}

Game::~Game() { }

sf::Time Game::GetElapsed() { 
	return m_elapsed; 
}
void Game::RestartClock() {
	m_elapsed = m_clock.restart(); 

}
C_PlayerManager * Game::GetPlayerManager() const
{
	return playerManager;
}
float Game::distance2Point(float x, float y, float a, float b)
{
	float dis = sqrt((x - a) * (x - a) + (y - b) * (y - b));
	return dis;
}
WindowGame* Game::GetWindow() { return &m_window; }

void Game::HandleInput() {
	if (!m_window.isPause)
	{
		// Input handling
		//Player move left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && m_player.getPosition().x > 0)
		{
			m_player.setPosition(m_player.getPosition().x - m_player.GetSpeed(), m_player.getPosition().y);
			
		}
		//Player move right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && m_player.getPosition().x < m_window.GetWindowSize().x)
		{
			m_player.setPosition(m_player.getPosition().x + m_player.GetSpeed(), m_player.getPosition().y);

		}
		//Player move up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_player.getPosition().y < m_window.GetWindowSize().y)
		{
			m_player.setPosition(m_player.getPosition().x, m_player.getPosition().y + m_player.GetSpeed());

		}
		//Player move down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_player.getPosition().y > 0)
		{
			m_player.setPosition(m_player.getPosition().x, m_player.getPosition().y - m_player.GetSpeed());

		}

		//Player fire
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && cooldownBullet.getElapsedTime().asMilliseconds() >= sf::seconds(0.3f).asMilliseconds())
		{
			sf::Vector2f posBullet(m_player.getPosition().x, m_player.getPosition().y);
			sf::Vector2f dirBullet((sf::Vector2f)sf::Mouse::getPosition(m_window.m_window) - posBullet);

			Bullet* bullet = new Bullet(posBullet, m_player.GetTeamIndex());

			bullet->SetBulletDirection((sf::Vector2f)sf::Mouse::getPosition(m_window.m_window) - posBullet);

			bullets.push_back(bullet);

			cooldownBullet.restart();

			//Create a fire packet
			sf::Packet* p = new sf::Packet();
			
			CreateFirePacket(*p, posBullet, dirBullet, bullet->GetBulletOfTeam());

			listPacket.push(p);
		}
	}

	//Create a move packet
	if (distance2Point(m_player.getPosition().x, m_player.getPosition().y, prePosition.x, prePosition.y) > 10.0f)
	{
		sf::Packet* p = new sf::Packet();

		CreateMovePacket(*p, m_player.getPosition().x, m_player.getPosition().y);

		listPacket.push(p);

		prePosition = m_player.getPosition();
	}
}

void Game::Update() {
	m_window.Update();

	sf::Lock lock(m_mutex);

	//Update bullet
	for (int i = 0; i < bullets.size(); i++)
	{
		Bullet* b = bullets.at(i);
		if (!b->isExist())
		{
			bullets.erase(bullets.begin() + i);
			continue;
		}

		b->Move(b->GetBulletDirection(),  b->GetSpeed() * m_elapsed.asSeconds());
	}
}


void Game::Render() {
	m_window.BeginDraw(); // Clear.

	//sf::View view;

	////view.setCenter(m_player.getPosition());
	//view.setSize(m_window.GetWindowSize().x * 2, m_window.GetWindowSize().y * 2);
	////m_window.SetPosView(m_player.getPosition());
	//m_window.SetView(view);
	


	//Draw players
	for (auto itr = playerManager->players.begin(); itr != playerManager->players.end(); itr++)
	{
		m_window.Draw(itr->second->getSprite());
	}

	m_window.Draw(m_player.getCircleSprite());

	//Draw bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		m_window.Draw(bullets.at(i)->GetBulletSprite());
	}


	
	//m_window.Draw(m_player.getSprite());
	m_window.EndDraw(); // Display.
}

void Game::Broadcast(Client* l_client)
{
	//Update to the server
	if (!listPacket.empty())
	{
		for (int i = 0; i < listPacket.size(); i++)
		{
			sf::Packet p = *listPacket.front();
			l_client->Send(p);
			
			listPacket.pop();
		}
	}
}
