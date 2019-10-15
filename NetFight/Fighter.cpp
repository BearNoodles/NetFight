#include "Fighter.h"



Fighter::Fighter(sf::Vector2f position)
{
	m_position = position;
	m_hurtBox.setSize(sf::Vector2f(100, 200));
	m_hurtBox.setFillColor(sf::Color::Red);
	
}

void Fighter::UpdateFrame()
{
	if (m_currentInput.inputs[2])
	{
		m_position.x -= 1;
	}
	else if (m_currentInput.inputs[3])
	{
		m_position.x += 1;
	}

	m_hurtBox.setPosition(m_position);
}

sf::RectangleShape Fighter::GetHurtbox()
{
	return m_hurtBox;
}

void Fighter::SetInput(FrameInput input)
{
	m_currentInput = input;
}

Fighter::~Fighter()
{
}
