#include "Menu.h"

Menu::Menu(sf::RenderWindow* wind)
{
	m_wind = wind;

	if (!tx1.loadFromFile("images/buttup.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}

	if (!tx2.loadFromFile("images/buttdown.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}

	if (!tx3.loadFromFile("images/box.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}

	//if (!tx3.loadFromFile("images/box.png"))
	//{
	//	// error...
	//	std::cout << "error loading image.png" << std::endl;
	//}

	if (!tickTexture.loadFromFile("images/tick.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	
	sp1.setTexture(tx1);
	sp2.setTexture(tx2);
	sp3.setTexture(tx3);
	sp3.setScale(sf::Vector2f(0.1f, 0.1f));
	tickSprite.setTexture(tickTexture);
	tickSprite.setScale(sf::Vector2f(0.26f, 0.26f));

	//load and set font
	if (!buttonFont.loadFromFile("font.ttf"))
	{
		// error...
		std::cout << "Load font failed" << std::endl;
	}
	else
	{
		startText.setFont(buttonFont);
		startText.setCharacterSize(26);
		startText.setString("Start");

		hostText.setFont(buttonFont);
		hostText.setCharacterSize(26);
		hostText.setString("Host");

		clientText.setFont(buttonFont);
		clientText.setCharacterSize(26);
		clientText.setString("Client");

		readyText.setFont(buttonFont);
		readyText.setCharacterSize(26);
		readyText.setString("Ready");

		searchingText.setFont(buttonFont);
		searchingText.setCharacterSize(26);
		searchingText.setString("Searching...");

		rollbackText.setFont(buttonFont);
		rollbackText.setCharacterSize(26);
		rollbackText.setString("Use Rollback");
		rollbackText.setOutlineColor(sf::Color::Black);
		rollbackText.setOutlineThickness(5.0f);

		offlineText.setFont(buttonFont);
		offlineText.setCharacterSize(26);
		offlineText.setString("Offline");
	}

	
	m_startButton = new MenuButton(sp1, sp2, startText, m_wind);
	m_hostButton = new MenuButton(sp1, sp2, hostText, m_wind);
	m_clientButton = new MenuButton(sp1, sp2, clientText, m_wind);
	m_readyButton = new MenuButton(sp1, sp2, readyText, m_wind);
	m_rollbackButton = new MenuButton(sp1, sp2, rollbackText, m_wind);
	m_offlineButton = new MenuButton(sp1, sp2, offlineText, m_wind);
	

	m_startButton->SetButtonPosition(sf::Vector2f(100.0f, 100.0f));
	m_hostButton->SetButtonPosition(sf::Vector2f(100.0f, 200.0f));
	m_clientButton->SetButtonPosition(sf::Vector2f(600.0f, 200.0f));
	m_readyButton->SetButtonPosition(sf::Vector2f(300.0f, 200.0f));
	m_rollbackButton->SetButtonPosition(sf::Vector2f(600.0f, 50.0f));
	m_offlineButton->SetButtonPosition(sf::Vector2f(400.0f, 100.0f));

	m_rollbackButton->SetTextPosition(sf::Vector2f(-150, 10));

	tickSprite.setPosition(sf::Vector2f(600.0f, 30.0f));

	isMenuFinished = false;
	isReadyToMatch = false;
	isReadyToPlay = false;
	

	player = 0;
	state = start;

	rollBackOn = false;
}

void Menu::UpdateMenu()
{
	if (state == start)
	{
		m_startButton->UpdateButton();
		m_offlineButton->UpdateButton();
		if (m_startButton->Pressed())
		{
			state = hostClient;
		}
		else if (m_offlineButton->Pressed())
		{
			startOffline = true;
		}
	}
	else if (state == hostClient)
	{
		m_hostButton->UpdateButton();
		m_clientButton->UpdateButton();
		if (m_hostButton->Pressed())
		{
			player = 1;
			state = searching;
			isReadyToMatch = true;
			//isMenuFinsiehd = true;
		}
		else if (m_clientButton->Pressed())
		{
			player = 2;
			state = searching;
			isReadyToMatch = true;
			//isMenuFinsiehd = true;
		}
	}

	else if (state == searching)
	{
		//Draw "Finding opponent"
	}

	else if (state == ready)
	{
		isReadyToMatch = false;
		if (!isReadyToPlay)
		{
			isReadyToPlay = true;
		}

		m_readyButton->UpdateButton();

		if (player == 1)
		{
			m_rollbackButton->UpdateButton();
		}

		if (m_rollbackButton->Pressed())
		{
			rollBackOn = !rollBackOn;
		}


		if (player == 2 || m_readyButton->Pressed())
		{
			isMenuFinished = true;
		}
	}
}

void Menu::DrawMenu()
{
	if (state == start)
	{
		m_wind->draw(m_startButton->GetCurrentSprite());
		m_wind->draw(m_startButton->GetButtonText());

		m_wind->draw(m_offlineButton->GetCurrentSprite());
		m_wind->draw(m_offlineButton->GetButtonText());
	}

	if (state == hostClient)
	{
		m_wind->draw(m_hostButton->GetCurrentSprite());
		m_wind->draw(m_hostButton->GetButtonText());

		m_wind->draw(m_clientButton->GetCurrentSprite());
		m_wind->draw(m_clientButton->GetButtonText());
	}
	if (state == searching)
	{
		m_wind->draw(searchingText);
	}

	if(state == ready)
	{
		if (player == 1)
		{
			m_wind->draw(m_readyButton->GetCurrentSprite());
			m_wind->draw(m_readyButton->GetButtonText());

			m_wind->draw(m_rollbackButton->GetCurrentSprite());
			m_wind->draw(m_rollbackButton->GetButtonText());

			if (rollBackOn)
			{
				m_wind->draw(tickSprite);
			}
		}
		else
		{
			//DRAW WAITING TEXT 
		}
	}
}

int Menu::GetHostOrClient()
{
	return player;
}

bool Menu::GetSearching()
{
	return isReadyToMatch;
}

bool Menu::GetReady()
{
	return isReadyToPlay;
}

int Menu::GetPlayer()
{
	return player;
}

void Menu::SetReady()
{
	state = ready;
	isReadyToMatch = false;
}

bool Menu::StartGame()
{
	if (player == 2)
	{
		return true;
	}
	return isMenuFinished;
}

bool Menu::GetRollback()
{
	return rollBackOn;
}

bool Menu::GetStartOffline()
{
	return startOffline;
}

