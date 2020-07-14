#include "MenuButton.h"

MenuButton::MenuButton(sf::Sprite uSprite, sf::Sprite dSprite, sf::RenderWindow* wind)
{
	m_upSprite = uSprite;
	m_downSprite = dSprite;

	m_currentSprite = m_upSprite;

	m_wind = wind;

	m_isDown = false;

	pressed = false;
}

void MenuButton::UpdateButton()
{
	sf::Vector2f pixelPos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_wind));
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (m_currentSprite.getGlobalBounds().contains(pixelPos) && !m_isDown)// m_rect->getGlobalBounds().contains(pixelPos))
		{
			m_currentSprite = m_downSprite;
			m_isDown = true;
			std::cout << "button down" << std::endl;
		}
	}
	else if(m_isDown)
	{
		m_currentSprite = m_upSprite;
		m_isDown = false;
		std::cout << "button up" << std::endl;
		pressed = true;
	}
}

sf::Sprite MenuButton::GetCurrentSprite()
{
	
	return m_currentSprite;
}

void MenuButton::SetButtonPosition(sf::Vector2f position)
{
	m_upSprite.setPosition(position);
	m_downSprite.setPosition(position);
	m_currentSprite.setPosition(position);
}

bool MenuButton::Pressed()
{
	bool isPressed = pressed;
	pressed = false;
	return isPressed;
}


