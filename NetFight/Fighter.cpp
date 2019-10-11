#include "Fighter.h"



Fighter::Fighter(sf::Vector2f position)
{
	m_position = position;
	m_hurtBox.setSize(sf::Vector2f(100, 200));
	m_hurtBox.setFillColor(sf::Color::Red);
	
}

void Fighter::UpdateFrame()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_position.x -= 1;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_position.x += 1;
	}

	m_hurtBox.setPosition(m_position);
}

sf::RectangleShape Fighter::GetHurtbox()
{
	return m_hurtBox;
}

Fighter::~Fighter()
{
}
