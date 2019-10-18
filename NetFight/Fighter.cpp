#include "Fighter.h"



Fighter::Fighter(sf::Vector2f position, int playerNumber, int screenwidth)
{
	m_position = position;
	m_hurtbox.setSize(sf::Vector2f(100, 200));
	m_hurtbox.setFillColor(sf::Color::Green);
	//m_activeHitbox = nullptr;
	m_activeHitbox = nullptr;
	m_hitboxPosition = sf::Vector2f(0, 0);
	m_isHitboxActive = false;

	m_characterData.LoadCharacter1();

	m_characterActions = m_characterData.GetCharacterStruct();

	m_attackState = idle;

	m_hitStunFrames = 0;

	m_direction = (playerNumber * -2) + 3;

	m_screenWidth = screenwidth;
}

void Fighter::UpdateFrame()
{
	//CHANGE UPDATE TO ITERATE THROUGH CURRENTATTACKFRAME IF NOT IDLE
	//AND CHECK HOW MANY FRAMES THE CURRENT ATTACK STATE HAS FROM THE CHARACTRE DATA STRUCT
	if (m_attackState == attacking)
	{
		m_actionFrame++;
		
		if (m_actionFrame >= m_currentAction.recoveryFrames + m_currentAction.activeFrames + m_currentAction.startupFrames)
		{
			m_attackState = idle;
			m_actionFrame = 0;
		}
		else if (m_actionFrame == m_currentAction.activeFrames + m_currentAction.startupFrames)
		{
			RemoveActiveHitbox();
		}
		else if (m_actionFrame == m_currentAction.startupFrames)
		{
			m_activeHitbox = &m_currentAction.activeHitbox;
			m_isHitboxActive = true;
			m_activeHitbox->setPosition(m_position + m_activeHitbox->getPosition());
		}
	}

	else if (m_attackState == hit)
	{
		if (m_activeHitbox != nullptr)
		{
			RemoveActiveHitbox();
		}

		m_actionFrame++;

		//ADD PUSHBACK TO ATTACKING PLAYER
		if(m_position.x > 0 && m_position.x < m_screenWidth - m_hurtbox.getSize().x)
			m_position.x -= 3 * m_direction;

		if (m_actionFrame >= m_hitStunFrames)
		{
			m_attackState = idle;
			m_actionFrame = 0;
		}

	}

	else
	{
		if (m_currentInput.inputs[2] && m_position.x > 0)
		{
			m_position.x -= 10 * m_direction;
		}
		else if (m_currentInput.inputs[3] && m_position.x < m_screenWidth - m_hurtbox.getSize().x)
		{
			m_position.x += 10 * m_direction;
		}

		if (m_currentInput.inputs[0])
		{
			m_currentAction = m_characterActions.attack1;
			m_attackState = attacking;
		}


		//sf::Vector2f pos = m_position + m_hitboxPosition;
		//m_activeHitbox->setPosition(pos);
	}


	m_hurtbox.setPosition(m_position);
}

void Fighter::HandleCollision(Action opponentAttack)
{
	m_attackState = hit;
	m_actionFrame = 0;
	m_hitStunFrames = opponentAttack.hitstun;
}

bool Fighter::CheckForHit(sf::RectangleShape* opponentHitbox)
{
	if (m_hurtbox.getGlobalBounds().intersects(opponentHitbox->getGlobalBounds()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Fighter::RemoveActiveHitbox()
{
	m_activeHitbox = nullptr;
	m_isHitboxActive = false;
}

sf::RectangleShape Fighter::GetHurtbox()
{
	return m_hurtbox;
}

Action Fighter::GetCurrentAction()
{
	return m_currentAction;
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
