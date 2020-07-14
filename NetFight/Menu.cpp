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

	m_startButton = new MenuButton(sp1, sp2, m_wind);
	m_hostButton = new MenuButton(sp1, sp2, m_wind);
	m_clientButton = new MenuButton(sp1, sp2, m_wind);

	m_startButton->SetButtonPosition(sf::Vector2f(100.0f, 100.0f));
	m_hostButton->SetButtonPosition(sf::Vector2f(100.0f, 200.0f));
	m_clientButton->SetButtonPosition(sf::Vector2f(600.0f, 200.0f));

	isMenuFinsiehd = false;

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
			isMenuFinsiehd = true;
		}
		else if (m_clientButton->Pressed())
		{
			player = 2;
			isMenuFinsiehd = true;
		}
	}
}

void Menu::DrawMenu()
{
	if (state == start)
	{
		m_wind->draw(m_startButton->GetCurrentSprite());
	}

	if (state == hostClient)
	{
		m_wind->draw(m_hostButton->GetCurrentSprite());
		m_wind->draw(m_clientButton->GetCurrentSprite());
	}
}

int Menu::GetHostOrClient()
{
	return player;
}
