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

	sf::Sprite GetCurrentSprite();
	sf::RectangleShape* GetRect();

private:
	sf::RectangleShape* m_rect;

	sf::Sprite m_currentSprite;
	sf::Sprite m_downSprite;
	sf::Sprite m_upSprite;

	sf::RenderWindow* m_wind;

	bool m_isDown;



};

