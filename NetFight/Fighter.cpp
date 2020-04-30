#include "Fighter.h"



Fighter::Fighter(sf::Vector2i position, int playerNumber, int screenwidth, int floorHeight)
{

	m_playerID = playerNumber;

	m_direction = (playerNumber * -2) + 3;

	m_startPosition = position;

	m_walkSpeed = 6;
	m_screenWidth = screenwidth;
	m_initialJumpSpeed = sf::Vector2i(m_walkSpeed, 13);
	m_jumpSpeedLossRate = 2;

	m_floorPosition = floorHeight;
	//m_landPosition = m_floorPosition;

	m_maxHealth = 1000;

	Reset();
}

void Fighter::Reset()
{
	m_position = m_startPosition;
	m_hurtbox.setSize((sf::Vector2f)sf::Vector2i(100, 200));

	if (m_direction == 1)
	{
		m_hurtboxOffset = sf::Vector2i(30, 30);
	}
	else
	{
		m_hurtboxOffset = sf::Vector2i(-130, 30);
	}

	m_position.y = m_position.y - (m_hurtbox.getSize().y);

	m_activeHitbox = nullptr;
	m_hitboxPosition = sf::Vector2i(0, 0);
	m_isHitboxActive = false;

	m_stunFrames = 0;

	m_hitLanded = false;

	m_jumpSpeed = sf::Vector2i(0, 0);

	m_currentHealth = m_maxHealth;

	m_actionFrame = 0;
	m_animFrame = 0;
	m_animRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
}

void Fighter::SetCharacterData(CharacterStruct data)
{
	m_characterActions = data;
	m_currentAnim = m_characterActions.idleAnim;

	m_currentAction = m_characterActions.idle;
	m_currentAnim = m_characterActions.idleAnim;
	m_spriteSheet = m_currentAnim.spriteSheet;


	ChangeState(idle);
}

void Fighter::UpdateFrame()
{
	m_actionFrame++;
	m_animFrame++;
	if (m_animFrame > m_currentAction.framesT)
	{
		if (!m_currentAnim.loop)
		{
			m_animFrame--;
		}
		else
		{
			m_animFrame = 1;
		}
	}

	//CHANGE UPDATE TO ITERATE THROUGH CURRENTATTACKFRAME IF NOT IDLE
	//AND CHECK HOW MANY FRAMES THE CURRENT ATTACK STATE HAS FROM THE CHARACTRE DATA STRUCT
	if (m_playerState == attack1 || m_playerState == attack2)
	{
		//m_actionFrame++;
		
		//TODO:
		//Update this to push attacking player on block too
		if (m_hitLanded && m_pushbackFrame < m_currentAction.selfHitPushback)
		{
			m_pushbackFrame++;
			m_position.x -= m_direction * m_currentAction.selfHitPushback;
		}
		
		if (m_actionFrame >= m_currentAction.framesT)
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

		//m_actionFrame++;


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
	
	else if (m_playerState == jumpU || m_playerState == jumpF || m_playerState == jumpB || m_playerState == jumpHit)
	{
		UpdateJump();
	}

	
	else
	{
		if (m_currentInput.inputs[2])// && m_position.x > 0)
		{
			if (m_direction == 1 && m_playerState != walkB)
			{
				ChangeState(walkB);
			}
			else if(m_direction == -1 && m_playerState != walkF)
			{
				ChangeState(walkF);
			}
		}
		else if (m_currentInput.inputs[3])// && m_position.x < m_screenWidth - m_hurtbox.getSize().x)
		{
			if (m_direction == 1 && m_playerState != walkF)
			{
				ChangeState(walkF);
			}
			else if(m_direction == -1 && m_playerState != walkB)
			{
				ChangeState(walkB);
			}
		}

		//Change to jump state the nset start jump there
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
			ChangeState(attack1);
		}

		else if (m_currentInput.inputs[5])
		{
			ChangeState(attack2);
		}

		//Check which state (idle walkf or walkb)
		if (m_playerState == walkB)
		{
			if ((m_direction == 1 && !m_currentInput.inputs[2]) || m_direction == -1 && !m_currentInput.inputs[3])
			{
				ChangeState(idle);
			}
			if (!IsCornered())
			{
				m_position.x -= m_walkSpeed * m_direction;
			}
		}
		else if (m_playerState == walkF)
		{
			if ((m_direction == 1 && !m_currentInput.inputs[3]) || m_direction == -1 && !m_currentInput.inputs[2])
			{
				ChangeState(idle);
			}
			if (IsCornered())
			{
				//m_position.x -= m_walkSpeed * m_direction;
			}
			m_position.x += m_walkSpeed * m_direction;
		}

		//sf::Vector2f pos = m_position + m_hitboxPosition;
		//m_activeHitbox->setPosition(pos);
	}

	offsetVec = sf::Vector2f(m_currentAnim.offsetX * m_direction, m_currentAnim.offsetY);

	m_hurtbox.setPosition((sf::Vector2f)m_position);
	if (m_playerState != jumpU && m_playerState != jumpF && m_playerState != jumpB)// && m_playerState != jumpHit)
	{
		m_spritePosition = sf::Vector2i((sf::Vector2f)m_position + offsetVec);
	}

	m_spriteSheet->setPosition((sf::Vector2f)m_spritePosition - (sf::Vector2f)m_hurtboxOffset);
	//updateanimation
	SetAnimRect();
}

void Fighter::SetAnimRect()
{
	int frames = m_currentAction.framesT;
	int width = m_currentAnim.spriteWidth;
	int height = m_currentAnim.spriteHeight;
	int frameWidth = width * m_currentAnim.framesPerStep / frames;
	int posX = (int)((m_animFrame - 1) / (m_currentAnim.framesPerStep)) * (frameWidth);
	int posY = m_currentAnim.offsetY;
	m_animRect = sf::IntRect(sf::Vector2i(posX, 0), sf::Vector2i(frameWidth, height));
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
	else if (m_playerState == jumpU || m_playerState == jumpF || m_playerState == jumpB || m_playerState == jumpHit)
	{
		ChangeState(jumpHit);
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

sf::Sprite Fighter::GetAnimationFrame()
{
	m_spriteSheet->setTextureRect(m_animRect);
	return *m_spriteSheet;
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
	//return (m_position.x + m_hurtboxOffset.x <= 0 || m_position.x + m_hurtboxOffset.x >= m_screenWidth - m_hurtbox.getSize().x);
	return (m_position.x <= 0 || m_position.x >= m_screenWidth - m_hurtbox.getSize().x);
}

void Fighter::ChangeState(PlayerState playerState)
{
	m_playerState = playerState;
	m_hitLanded = false;
	m_actionFrame = 0;
	m_animFrame = 0;
	m_pushbackFrame = 0;
	sf::Color greenish = sf::Color::Green;
	sf::Color pinkish = sf::Color::Magenta;
	sf::Color yellowish = sf::Color::Yellow;
	sf::Color blueish = sf::Color::Blue;
	greenish.a = 150;
	pinkish.a = 150;
	yellowish.a = 150;
	blueish.a = 150;
	switch (m_playerState)
	{
		case idle:
			m_currentAction = m_characterActions.idle;
			m_currentAnim = m_characterActions.idleAnim;
			m_hurtbox.setFillColor(greenish);
			break;
		case walkF:
			m_currentAction = m_characterActions.walkF;
			m_currentAnim = m_characterActions.walkFAnim;
			m_hurtbox.setFillColor(greenish);
			break;
		case walkB:
			m_currentAction = m_characterActions.walkB;
			m_currentAnim = m_characterActions.walkBAnim;
			m_hurtbox.setFillColor(greenish);
			break;
		case attack1:
			m_currentAction = m_characterActions.attack1;
			m_currentAnim = m_characterActions.attackAnim1;
			m_hurtbox.setFillColor(pinkish);
			break;
		case attack2:
			m_currentAction = m_characterActions.attack2;
			m_currentAnim = m_characterActions.attackAnim2;
			m_hurtbox.setFillColor(pinkish);
			break;
		case hit:
			m_currentAction = m_characterActions.hit;
			m_currentAnim = m_characterActions.hitAnim;
			m_hurtbox.setFillColor(yellowish);
			break;
		case block:
			m_currentAction = m_characterActions.block;
			m_currentAnim = m_characterActions.blockAnim;
			m_hurtbox.setFillColor(blueish);
			break; 
		case jumpU:
			m_currentAction = m_characterActions.jumpU;
			m_currentAnim = m_characterActions.jumpUAnim;
			m_hurtbox.setFillColor(greenish);
			break;
		case jumpF:
			m_currentAction = m_characterActions.jumpF;
			m_currentAnim = m_characterActions.jumpFAnim;
			m_hurtbox.setFillColor(greenish);
			break;
		case jumpB:
			m_currentAction = m_characterActions.jumpB;
			m_currentAnim = m_characterActions.jumpBAnim;
			m_hurtbox.setFillColor(greenish);
			break;
		case jumpHit:
			m_currentAction = m_characterActions.jumpHit;
			m_currentAnim = m_characterActions.jumpHitAnim;
			m_stunFrames = 6000;
			m_hurtbox.setFillColor(greenish);
			break;
		default:
			m_currentAction = m_characterActions.idle;
			m_currentAnim = m_characterActions.idleAnim;
			m_hurtbox.setFillColor(greenish);
			break;
	}

	m_spriteSheet = m_currentAnim.spriteSheet;
	m_spriteSheet->setScale(sf::Vector2f(m_direction, 1));
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
		if (m_playerState == jumpU || m_playerState == jumpF || m_playerState == jumpB)
		{
			m_spritePosition.x -= (m_walkSpeed / 2) * m_direction;
		}
	}
}

void Fighter::StartJump(int direction)
{
	if (direction == 0)
	{
		ChangeState(jumpU);
	}
	else if (direction == m_direction)
	{
		ChangeState(jumpF);
	}
	else
	{
		ChangeState(jumpB);
	}
	//m_jumpSpeed = sf::Vector2i(m_initialJumpSpeed.x * m_direction * direction, -m_initialJumpSpeed.y);
	m_jumpSpeed = sf::Vector2i(m_initialJumpSpeed.x * direction, -m_initialJumpSpeed.y);

	offsetVec = sf::Vector2f(m_currentAnim.offsetX * m_direction, m_currentAnim.offsetY);
	m_spritePosition = sf::Vector2i((sf::Vector2f)m_position + offsetVec);
}

void Fighter::UpdateJump()
{
	if (m_actionFrame < m_characterActions.preJumpFrames)
	{
		return;
	}
	if (m_jumpSpeed.x != 0 && !IsCornered())// m_position.x < m_screenWidth - m_hurtbox.getSize().x))
	{
		m_position.x += m_jumpSpeed.x;
	}
	/*else if (m_jumpSpeed.x * m_direction > 0)
	{
		m_position.x += m_jumpSpeed.x;
	}*/
	else if (m_jumpSpeed.x * m_direction < 0 && IsCornered())
	{
		m_spritePosition.x -= m_jumpSpeed.x;
	}
	else
	{
		m_position.x += m_jumpSpeed.x;
	}
	m_position.y += m_jumpSpeed.y;

	if (m_actionFrame % 2 == 0)
	{
		m_jumpSpeed.y++;
	}

	if (m_position.y + m_hurtbox.getSize().y >= m_floorPosition)
	{
		ChangeState(idle);
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

		m_hitLanded = gameState.player1HitLanded;
		m_currentAction = gameState.player1Action;
		m_currentAnim = gameState.player1Anim;
		m_actionFrame = gameState.player1ActionFrame;
		m_animFrame = gameState.player1AnimFrame;
		m_currentHealth = gameState.player1Health;
		m_position = gameState.player1Position;
		m_spritePosition = gameState.player1SpritePosition;
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

		m_hitLanded = gameState.player2HitLanded;
		m_currentAction = gameState.player2Action;
		m_currentAnim = gameState.player2Anim;
		m_actionFrame = gameState.player2ActionFrame;
		m_animFrame = gameState.player2AnimFrame;
		m_currentHealth = gameState.player2Health;
		m_position = gameState.player2Position;
		m_spritePosition = gameState.player2SpritePosition;
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

	SetAnimRect();
}

GameState Fighter::GetFighterState()
{
	GameState currentState;

	if (m_playerID == 1)
	{
		currentState.player1HitLanded = m_hitLanded;
		currentState.player1Action = m_currentAction;
		currentState.player1Anim = m_currentAnim;
		currentState.player1ActionFrame = m_actionFrame;
		currentState.player1AnimFrame = m_animFrame;
		currentState.player1Health = m_currentHealth;
		currentState.player1Position = m_position;
		currentState.player1SpritePosition = m_spritePosition;
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
		currentState.player2HitLanded = m_hitLanded;
		currentState.player2Action = m_currentAction;
		currentState.player2Anim = m_currentAnim;
		currentState.player2ActionFrame = m_actionFrame;
		currentState.player2AnimFrame = m_animFrame;
		currentState.player2Health = m_currentHealth;
		currentState.player2Position = m_position;
		currentState.player2SpritePosition = m_spritePosition;
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

int Fighter::GetDirection()
{
	return m_direction;
}

Fighter::~Fighter()
{

}
