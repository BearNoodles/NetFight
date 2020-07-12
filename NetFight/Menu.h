#pragma once
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
class Menu
{
public:
	Menu(sf::RenderWindow* wind);
	~Menu();

	void DrawButton();

	void UpdateMenu();


private:
	//Menu button
	MenuButton* button;

	sf::RenderWindow* m_wind;

	sf::Texture tx1;
	sf::Texture tx2;
	sf::Sprite sp1;
	sf::Sprite sp2;
};

