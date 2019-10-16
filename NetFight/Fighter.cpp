#include "Fighter.h"



Fighter::Fighter(sf::Vector2f position)
{
	m_position = position;
	m_hurtbox.setSize(sf::Vector2f(100, 200));
	m_hurtbox.setFillColor(sf::Color::Green);
	//m_activeHitbox = nullptr;
	m_activeHitbox = new sf::RectangleShape();
	m_hitboxPosition = sf::Vector2f(0, 0);
	m_isHitboxActive = false;

	m_characterData.LoadCharacter1();
}

void Fighter::UpdateFrame()
{
	//CHANGE UPDATE TO ITERATE THROUGH CURRENTATTACKFRAME IF NOT IDLE
	//AND CHECK HOW MANY FRAMES THE CURRENT ATTACK STATE HAS FROM THE CHARACTRE DATA STRUCT
	if (m_currentInput.inputs[2])
	{
		m_position.x -= 1;
	}
	else if (m_currentInput.inputs[3])
	{
		m_position.x += 1;
	}

	if (m_currentInput.inputs[0])
	{
		CreateHitbox();
		m_isHitboxActive = true;
	}

	m_hurtbox.setPosition(m_position);

	sf::Vector2f pos = m_position + m_hitboxPosition;
	m_activeHitbox->setPosition(pos);
}

sf::RectangleShape Fighter::GetHurtbox()
{
	return m_hurtbox;
}



void Fighter::SetInput(FrameInput input)
{
	m_currentInput = input;
}

sf::RectangleShape Fighter::GetActiveHitbox()
{
	return  *m_activeHitbox;
}

void Fighter::CreateHitbox()
{
	m_activeHitbox = new sf::RectangleShape();
	m_activeHitbox->setSize(sf::Vector2f(100, 50));
	m_activeHitbox->setFillColor(sf::Color::Red);

	m_hitboxPosition = sf::Vector2f(100, 50);

	m_activeHitbox->setPosition(m_position + m_hitboxPosition);
}

bool Fighter::IsHitboxActive()
{
	return m_isHitboxActive;
}

void Fighter::ChangeState(State attackState)
{
	m_attackState = attackState;
}

Fighter::~Fighter()
{

}
