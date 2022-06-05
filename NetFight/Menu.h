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

	bool GetStartOffline();
	void SetStartOffline();

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
	MenuButton* m_playOfflineButton;
	MenuButton* m_offlineButton;

	sf::Text startText, hostText, clientText, readyText, searchingText, rollbackText, offlineText;

	sf::RenderWindow* m_wind;

	sf::Texture tx1;
	sf::Texture tx2;
	sf::Texture tx3;
	sf::Texture tx4;
	sf::Sprite sp1;
	sf::Sprite sp2;
	sf::Sprite sp3;
	sf::Sprite sp4;

	sf::Texture tickTexture;
	sf::Sprite tickSprite;

	sf::Font buttonFont;

	bool isMenuFinished;
	bool isReadyToMatch;
	bool isReadyToPlay;
	bool isPlayOffline;
	bool startOffline;

	enum MenuState{start, hostClient, searching, ready};
	MenuState state;

	int player;

	bool rollBackOn;
};

