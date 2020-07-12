#include "Menu.h"

Menu::Menu(sf::RenderWindow* wind)
{
	m_wind = wind;

	if (!tx1.loadFromFile("kenJab.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}

	if (!tx2.loadFromFile("kenHit.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	sp1.setTexture(tx1);
	sp2.setTexture(tx2);

	button = new MenuButton(sp1, sp2, m_wind);
}

void Menu::UpdateMenu()
{
	button->UpdateButton();
}

void Menu::DrawButton()
{
	m_wind->draw(button->GetCurrentSprite());
	m_wind->draw(*button->GetRect());
}

