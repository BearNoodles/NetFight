#include "Fighter.h"



Fighter::Fighter(sf::Vector2f position, int playerNumber, int screenwidth)
{
	m_position = position;
	m_hurtbox.setSize(sf::Vector2f(100, 200));
	ChangeState(idle);
	//m_activeHitbox = nullptr;
	m_activeHitbox = nullptr;
	m_hitboxPosition = sf::Vector2f(0, 0);
	m_isHitboxActive = false;

	m_characterData.LoadCharacter1();

	m_characterActions = m_characterData.GetCharacterStruct();

	m_attackState = idle;

	m_stunFrames = 0;

	m_direction = (playerNumber * -2) + 3;

	m_screenWidth = screenwidth;

	m_hitLanded = false;
}

void Fighter::UpdateFrame()
{
	//CHANGE UPDATE TO ITERATE THROUGH CURRENTATTACKFRAME IF NOT IDLE
	//AND CHECK HOW MANY FRAMES THE CURRENT ATTACK STATE HAS FROM THE CHARACTRE DATA STRUCT
	if (m_attackState == attacking)
	{
		m_actionFrame++;

		if (m_hitLanded && m_pushbackFrame < m_currentAction.selfBlockPushback)
		{
			m_pushbackFrame++;
			m_position.x -= m_direction * m_currentAction.selfHitPushback;
		}
		
		if (m_actionFrame >= m_currentAction.recoveryFrames + m_currentAction.activeFrames + m_currentAction.startupFrames)
		{
			ChangeState(idle);
			m_hitLanded = false;
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

	else if (m_attackState == hit || m_attackState == block)
	{
		if (m_activeHitbox != nullptr)
		{
			RemoveActiveHitbox();
		}

		m_actionFrame++;


		if (!(IsCornered()) && m_pushbackFrame < m_hitBy.pushbackFames)
		{
			m_position.x -= m_pushback * m_direction;
		}

		if (m_actionFrame >= m_stunFrames)
		{
			ChangeState(idle);
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
			ChangeState(attacking);
		}


		//sf::Vector2f pos = m_position + m_hitboxPosition;
		//m_activeHitbox->setPosition(pos);
	}


	m_hurtbox.setPosition(m_position);
}

//POSSIBLY MAKE A BOOL SO OPPONENT KNOWS IF THE ATTACK WAS BLOCKED OR NOT
void Fighter::HandleCollision(Action opponentAttack)
{
	m_hitBy = opponentAttack;

	if ((m_attackState == idle || m_attackState == block) && ((m_direction == 1 && m_currentInput.inputs[2]) || (m_direction == -1 && m_currentInput.inputs[3])))
	{
		ChangeState(block);
		m_stunFrames = opponentAttack.blockstun;
		m_pushback = opponentAttack.blockPushback;
	}
	else
	{
		ChangeState(hit);
		m_stunFrames = opponentAttack.hitstun;
		m_pushback = opponentAttack.hitPushback;
	}

}

bool Fighter::CheckForHit(sf::RectangleShape* opponentHitbox)
{
	if (m_hurtbox.getGlobalBounds().intersects(opponentHitbox->getGlobalBounds()))
	{
		return true;
	}
	
	return false;
}

void Fighter::RemoveActiveHitbox()
{
	m_activeHitbox = nullptr;
	m_isHitboxActive = false;
}

void Fighter::HitLanded()
{
	RemoveActiveHitbox();
	m_pushbackFrame = 0;
	m_hitLanded = true;
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

bool Fighter::IsCornered()
{
	return (m_position.x <= 0 || m_position.x >= m_screenWidth - m_hurtbox.getSize().x);
}

void Fighter::ChangeState(State attackState)
{
	m_attackState = attackState;
	m_actionFrame = 0;
	switch (m_attackState)
	{
		case idle:
			m_hurtbox.setFillColor(sf::Color::Green);
			break;
		case attacking:
			m_hurtbox.setFillColor(sf::Color::Magenta);
			break;
		case hit:
			m_hurtbox.setFillColor(sf::Color::Yellow);
			break;
		case block:
			m_hurtbox.setFillColor(sf::Color::Blue);
			break;
		default: 
			m_hurtbox.setFillColor(sf::Color::Green);
			break;
	}
}

Fighter::~Fighter()
{

}
