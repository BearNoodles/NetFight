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

	bool GetSearching();
	bool GetReady();

private:
	//Menu button
	MenuButton* m_startButton;
	MenuButton* m_hostButton;
	MenuButton* m_clientButton;

	sf::Text startText, hostText, clientText;

	sf::RenderWindow* m_wind;

	sf::Texture tx1;
	sf::Texture tx2;
	sf::Sprite sp1;
	sf::Sprite sp2;

	sf::Font buttonFont;

	bool isMenuFinsiehd;
	bool isReadyToMatch;
	bool isReadyToPlay;

	enum MenuState{start, hostClient, searching, ready};
	MenuState state;

	int player;
};

