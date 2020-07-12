#include "MenuButton.h"

MenuButton::MenuButton(sf::Sprite uSprite, sf::Sprite dSprite, sf::RenderWindow* wind)
{
	m_upSprite = uSprite;
	m_downSprite = dSprite;

	m_currentSprite = m_upSprite;

	m_rect = new sf::RectangleShape();
	m_rect->setSize(sf::Vector2f(100.0f, 100.0f));
	m_rect->setFillColor(sf::Color::Black);
	//m_rect->setTexture(uSprite.getTexture());

	m_wind = wind;

	m_isDown = false;
}

void MenuButton::UpdateButton()
{
	sf::Vector2f pixelPos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_wind));
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_rect->getGlobalBounds().contains(pixelPos))
	{
		std::cout << "button down" << std::endl;
		m_currentSprite = m_downSprite;
	}
	else
	{
		//std::cout << "button up" << std::endl;
		m_currentSprite = m_upSprite;
	}
}

sf::Sprite MenuButton::GetCurrentSprite()
{
	
	return m_currentSprite;
}


sf::RectangleShape* MenuButton::GetRect()
{

	return m_rect;
}


