#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Input.h"

class MenuButton
{
public:
	MenuButton(sf::Sprite uSPrite, sf::Sprite dSprite, sf::RenderWindow* wind);
	~MenuButton();


	void UpdateButton();
	void SetButtonDown();
	bool Pressed();

	void SetButtonPosition(sf::Vector2f position);

	sf::Sprite GetCurrentSprite();



private:

	sf::Sprite m_currentSprite;
	sf::Sprite m_downSprite;
	sf::Sprite m_upSprite;

	sf::RenderWindow* m_wind;

	bool m_isDown;

	bool pressed;

};

