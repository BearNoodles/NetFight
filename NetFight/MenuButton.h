#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Input.h"

class MenuButton
{
public:
	MenuButton(sf::Sprite uSPrite, sf::Sprite dSprite, sf::Text text, sf::RenderWindow* wind);
	~MenuButton();


	void UpdateButton();
	void SetButtonDown();
	bool Pressed();

	void SetButtonPosition(sf::Vector2f position);

	sf::Sprite GetCurrentSprite();
	sf::Text GetButtonText();

	void SetTextPosition(sf::Vector2f pos);


private:

	sf::Sprite m_currentSprite;
	sf::Sprite m_downSprite;
	sf::Sprite m_upSprite;

	sf::Font m_font;
	sf::Text m_text;

	sf::RenderWindow* m_wind;

	bool m_isDown;

	bool pressed;

};

