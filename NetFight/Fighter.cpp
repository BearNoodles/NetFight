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
	m_playerID = playerNumber;

	m_walkSpeed = 6;

	m_characterData.LoadCharacter1();

	m_characterActions = m_characterData.GetCharacterStruct();

	m_playerState = idle;

	m_stunFrames = 0;

	m_direction = (playerNumber * -2) + 3;

	m_screenWidth = screenwidth;

	m_hitLanded = false;

	m_jumpSpeed = sf::Vector2i(0, 0);

	m_initialJumpSpeed = sf::Vector2i(m_walkSpeed, 20);

	m_floorPosition = position.y + m_hurtbox.getSize().y;

	m_maxHealth = 1000;
	m_currentHealth = m_maxHealth;
}

void Fighter::UpdateFrame()
{
	//CHANGE UPDATE TO ITERATE THROUGH CURRENTATTACKFRAME IF NOT IDLE
	//AND CHECK HOW MANY FRAMES THE CURRENT ATTACK STATE HAS FROM THE CHARACTRE DATA STRUCT
	if (m_playerState == attacking)
	{
		m_actionFrame++;

		//TODO:
		//Update this to push attacking player on block too
		if (m_hitLanded && m_pushbackFrame < m_currentAction.selfHitPushback)
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
			m_activeHitbox->setPosition(sf::Vector2f(m_position.x + m_activeHitbox->getPosition().x * m_direction, m_position.y + m_activeHitbox->getPosition().y));
		}
	}

	else if (m_playerState == hit || m_playerState == block)
	{
		if (m_activeHitbox != nullptr)
		{
			RemoveActiveHitbox();
		}

		m_actionFrame++;


		if (!(IsCornered()) && m_pushbackFrame < m_hitBy.pushbackFrames)
		{
			if (m_playerState == hit)
			{
				m_pushbackFrame++;
				m_position.x -= m_direction * m_hitBy.hitPushback;
			}
			else if (m_playerState == block)
			{
				m_pushbackFrame++;
				m_position.x -= m_direction * m_hitBy.blockPushback;
			}
		}

		if (m_actionFrame >= m_stunFrames)
		{
			ChangeState(idle);
		}
	}
	
	else if (m_playerState == jump)
	{
		UpdateJump();
	}

	else
	{
		if (m_currentInput.inputs[2] && m_position.x > 0)
		{
			m_position.x -= m_walkSpeed;// *m_direction;
		}
		else if (m_currentInput.inputs[3] && m_position.x < m_screenWidth - m_hurtbox.getSize().x)
		{
			m_position.x += m_walkSpeed;// *m_direction;
		}

		if (m_currentInput.inputs[0])
		{
			/*m_currentAction = m_characterActions.attack1;
			ChangeState(attacking);*/
			if (m_currentInput.inputs[2])
			{
				StartJump(-1);
			}
			else if (m_currentInput.inputs[3])
			{
				StartJump(1);
			}
			else
			{
				StartJump(0);
			}
		}

		if (m_currentInput.inputs[4])
		{
			m_currentAction = m_characterActions.attack1;
			ChangeState(attacking);
		}

		else if (m_currentInput.inputs[5])
		{
			m_currentAction = m_characterActions.attack2;
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

	if ((m_playerState == idle || m_playerState == block) && ((m_direction == 1 && m_currentInput.inputs[2]) || (m_direction == -1 && m_currentInput.inputs[3])))
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
		m_currentHealth -= m_hitBy.damage;
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

//void Fighter::CreateHitbox()
//{
//	m_activeHitbox = new sf::RectangleShape();
//	m_activeHitbox->setSize(sf::Vector2f(100, 50));
//	m_activeHitbox->setFillColor(sf::Color::Red);
//
//	m_hitboxPosition = sf::Vector2f(100, 50);
//
//	m_activeHitbox->setPosition(m_position + m_hitboxPosition);
//}

bool Fighter::IsHitboxActive()
{
	return m_isHitboxActive;
}

bool Fighter::IsCornered()
{
	return (m_position.x <= 0 || m_position.x >= m_screenWidth - m_hurtbox.getSize().x);
}

void Fighter::ChangeState(PlayerState playerState)
{
	m_playerState = playerState;
	m_actionFrame = 0;
	m_pushbackFrame = 0;
	switch (m_playerState)
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

bool Fighter::CheckPushing(sf::RectangleShape opponentHurtbox)
{
	if (m_hurtbox.getGlobalBounds().intersects(opponentHurtbox.getGlobalBounds()))
	{
		WalkPush();
		return true;
	}
	return false;
}

void Fighter::WalkPush()
{
	if (!IsCornered())
	{
		m_position.x -= (m_walkSpeed / 2) * m_direction;
	}
}

void Fighter::StartJump(int direction)
{
	m_playerState = jump;
	//m_jumpSpeed = sf::Vector2i(m_initialJumpSpeed.x * m_direction * direction, -m_initialJumpSpeed.y);
	m_jumpSpeed = sf::Vector2i(m_initialJumpSpeed.x * direction, -m_initialJumpSpeed.y);
}

void Fighter::UpdateJump()
{
	if ((m_jumpSpeed.x < 0 && m_position.x > 0) || (m_jumpSpeed.x > 0 && m_position.x < m_screenWidth - m_hurtbox.getSize().x))
	{
		m_position.x += m_jumpSpeed.x;
	}
	m_position.y += m_jumpSpeed.y;
	m_jumpSpeed.y++;
	if (m_position.y >= m_floorPosition - m_hurtbox.getSize().y)
	{
		m_playerState = idle;
		m_position.y = m_floorPosition - m_hurtbox.getSize().y;
		//m_verticalSpeed = 0;
	}
}

int Fighter::GetHealth()
{
	return m_currentHealth;
}

void Fighter::SetFighterState(GameState gameState)
{
	if (m_playerID == 1)
	{
		ChangeState(gameState.player1State);

		m_currentAction = gameState.player1Action;
		m_actionFrame = gameState.player1ActionFrame;
		m_currentHealth = gameState.player1Health;
		m_position = gameState.player1Position;
		m_playerState = gameState.player1State;
		m_isHitboxActive = gameState.player1IsHitboxActive;

		m_currentInput = gameState.player1CurrentInput;
		m_hitBy = gameState.player1HitBy;
		m_stunFrames = gameState.player1StunFrames;
		m_pushback = gameState.player1Pushback;
		m_jumpSpeed = gameState.player1JumpSpeed;
		m_direction = gameState.player1Direction;
		m_isBlocking = gameState.player1IsBlocking;
		m_pushbackFrame = gameState.player1PushbackFrame;

		if (m_isHitboxActive)
		{
			m_activeHitbox = &m_currentAction.activeHitbox;
		}
	}

	else if (m_playerID == 2)
	{
		ChangeState(gameState.player2State);

		m_currentAction = gameState.player2Action;
		m_actionFrame = gameState.player2ActionFrame;
		m_currentHealth = gameState.player2Health;
		m_position = gameState.player2Position;
		m_playerState = gameState.player2State;
		m_isHitboxActive = gameState.player2IsHitboxActive;

		m_currentInput = gameState.player2CurrentInput;
		m_hitBy = gameState.player2HitBy;
		m_stunFrames = gameState.player2StunFrames;
		m_pushback = gameState.player2Pushback;
		m_jumpSpeed = gameState.player2JumpSpeed;
		m_direction = gameState.player2Direction;
		m_isBlocking = gameState.player2IsBlocking;
		m_pushbackFrame = gameState.player2PushbackFrame;

		if (m_isHitboxActive)
		{
			m_activeHitbox = &m_currentAction.activeHitbox;
		}
	}
}

GameState Fighter::GetFighterState()
{
	GameState currentState;

	if (m_playerID == 1)
	{
		currentState.player1Action = m_currentAction;
		currentState.player1ActionFrame = m_actionFrame;
		currentState.player1Health = m_currentHealth;
		currentState.player1Position = m_position;
		currentState.player1State = m_playerState;
		currentState.player1IsHitboxActive = m_isHitboxActive;

		currentState.player1CurrentInput = m_currentInput;
		currentState.player1HitBy = m_hitBy;
		currentState.player1StunFrames = m_stunFrames;
		currentState.player1Pushback = m_pushback;
		currentState.player1JumpSpeed = m_jumpSpeed;
		currentState.player1Direction = m_direction;
		currentState.player1IsBlocking = m_isBlocking;
		currentState.player1PushbackFrame = m_pushbackFrame;
	}

	else if (m_playerID == 2)
	{
		currentState.player2Action = m_currentAction;
		currentState.player2ActionFrame = m_actionFrame;
		currentState.player2Health = m_currentHealth;
		currentState.player2Position = m_position;
		currentState.player2State = m_playerState;
		currentState.player2IsHitboxActive = m_isHitboxActive;

		currentState.player2CurrentInput = m_currentInput;
		currentState.player2HitBy = m_hitBy;
		currentState.player2StunFrames = m_stunFrames;
		currentState.player2Pushback = m_pushback;
		currentState.player2JumpSpeed = m_jumpSpeed;
		currentState.player2Direction = m_direction;
		currentState.player2IsBlocking = m_isBlocking;
		currentState.player2PushbackFrame = m_pushbackFrame;
	}

	return currentState;
}

Fighter::~Fighter()
{

}
