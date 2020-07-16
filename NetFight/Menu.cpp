#include "Menu.h"

Menu::Menu(sf::RenderWindow* wind)
{
	m_wind = wind;

	if (!tx1.loadFromFile("buttup.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}

	if (!tx2.loadFromFile("buttdown.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	
	sp1.setTexture(tx1);
	sp2.setTexture(tx2);

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
	}

	
	m_startButton = new MenuButton(sp1, sp2, startText, m_wind);
	m_hostButton = new MenuButton(sp1, sp2, hostText, m_wind);
	m_clientButton = new MenuButton(sp1, sp2, clientText, m_wind);
	

	m_startButton->SetButtonPosition(sf::Vector2f(100.0f, 100.0f));
	m_hostButton->SetButtonPosition(sf::Vector2f(100.0f, 200.0f));
	m_clientButton->SetButtonPosition(sf::Vector2f(600.0f, 200.0f));

	isMenuFinsiehd = false;
	isReadyToMatch = false;
	isReadyToPlay = false;

	player = 0;
	state = start;
}

void Menu::UpdateMenu()
{
	if (state == start)
	{
		m_startButton->UpdateButton();
		if (m_startButton->Pressed())
		{
			state = hostClient;
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

	}

	else if (state == ready)
	{
		if (!isReadyToPlay)
		{
			isReadyToPlay = true;
		}
	}
}

void Menu::DrawMenu()
{
	if (state == start)
	{
		m_wind->draw(m_startButton->GetCurrentSprite());
		m_wind->draw(m_startButton->GetButtonText());
	}

	if (state == hostClient)
	{
		m_wind->draw(m_hostButton->GetCurrentSprite());
		m_wind->draw(m_hostButton->GetButtonText());

		m_wind->draw(m_clientButton->GetCurrentSprite());
		m_wind->draw(m_clientButton->GetButtonText());
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
