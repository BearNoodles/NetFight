#pragma once
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
class Menu
{
public:
	Menu(sf::RenderWindow* wind);
	~Menu();

	void DrawMenu();

	void UpdateMenu();

	int GetHostOrClient();


private:
	//Menu button
	MenuButton* m_startButton;
	MenuButton* m_hostButton;
	MenuButton* m_clientButton;

	sf::RenderWindow* m_wind;

	sf::Texture tx1;
	sf::Texture tx2;
	sf::Sprite sp1;
	sf::Sprite sp2;

	bool isMenuFinsiehd;

	enum MenuState{start, hostClient};
	MenuState state;

	int player;
};

