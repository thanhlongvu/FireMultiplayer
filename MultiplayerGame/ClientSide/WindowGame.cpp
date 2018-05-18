#include "WindowGame.h"

WindowGame::WindowGame() { Setup("Window", sf::Vector2u(640, 480)); }
WindowGame::WindowGame(const std::string& title, const sf::Vector2u& size) { Setup(title, size); }
WindowGame::~WindowGame() { Destroy(); }

void WindowGame::Setup(const std::string title, const sf::Vector2u& size) {
	m_windowTitle = title;
	m_windowSize = size;
	m_isFullscreen = false;
	m_isDone = false;
	m_window.setFramerateLimit(60);
	Create();
}

void WindowGame::Create() {
	auto style = (m_isFullscreen ? sf::Style::Fullscreen
		: sf::Style::Default);
	m_window.create({ m_windowSize.x, m_windowSize.y, 32 },
		m_windowTitle, style);
}

void WindowGame::Destroy() {
	m_window.close();
}

void WindowGame::BeginDraw() { m_window.clear(sf::Color::Black); }
void WindowGame::EndDraw() { m_window.display(); }

bool WindowGame::IsDone() { return m_isDone; }
bool WindowGame::IsFullscreen() { return m_isFullscreen; }

void WindowGame::Draw(sf::Drawable& l_drawable) {
	m_window.draw(l_drawable);
}

sf::Vector2u WindowGame::GetWindowSize() { return m_windowSize; }

void WindowGame::ToggleFullscreen() {
	m_isFullscreen = !m_isFullscreen;
	Destroy();
	Create();
}

void WindowGame::Update() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) { m_isDone = true; }
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5) { ToggleFullscreen(); }

		if (event.type == sf::Event::LostFocus)
			isPause = true;

		if (event.type == sf::Event::GainedFocus)
			isPause = false;
	}
}