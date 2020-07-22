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
	void SetReady();

	int GetPlayer();

	bool GetRollback();

	bool StartGame();

private:
	//Menu button
	MenuButton* m_startButton;
	MenuButton* m_hostButton;
	MenuButton* m_clientButton;
	MenuButton* m_readyButton;
	MenuButton* m_rollbackButton;

	sf::Text startText, hostText, clientText, readyText, searchingText, rollbackText;

	sf::RenderWindow* m_wind;

	sf::Texture tx1;
	sf::Texture tx2;
	sf::Texture tx3;
	sf::Sprite sp1;
	sf::Sprite sp2;
	sf::Sprite sp3;

	sf::Texture tickTexture;
	sf::Sprite tickSprite;

	sf::Font buttonFont;

	bool isMenuFinished;
	bool isReadyToMatch;
	bool isReadyToPlay;

	enum MenuState{start, hostClient, searching, ready};
	MenuState state;

	int player;

	bool rollBackOn;
};

