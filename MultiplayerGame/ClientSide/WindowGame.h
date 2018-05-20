#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class WindowGame {
public:
	WindowGame();
	WindowGame(const std::string& title, const sf::Vector2u& size);
	~WindowGame();

	void BeginDraw();
	void EndDraw();

	void Update();

	bool IsDone();
	bool IsFullscreen();
	sf::Vector2u GetWindowSize();

	void ToggleFullscreen();

	void Draw(sf::Drawable& l_drawable);

	void SetPosView(const sf::Vector2f);

	void SetView(const sf::View&);

public:
	sf::RenderWindow m_window;
	bool isPause = false;

private:
	void Setup(const std::string title, const sf::Vector2u& size);
	void Create();
	void Destroy();
	
	sf::View m_view;

	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullscreen;
};