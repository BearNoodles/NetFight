#include "GameStateManager.h"

#include <iostream>


GameStateManager::GameStateManager()
{
	m_maxStateVectorSize = 500;
	m_gameStateVector = new std::vector<GameState>();
	m_emptyState.frame = -1; 
	for (int i = 0; i < m_maxStateVectorSize; i++)
	{
		m_emptyState.frame = i;
		m_gameStateVector->push_back(m_emptyState);
	}

	m_emptyState.frame = -1;
}

void GameStateManager::Reset()
{
	m_gameStateVector->clear();

	m_emptyState.frame = -1;
	for (int i = 0; i < m_maxStateVectorSize; i++)
	{
		m_emptyState.frame = i;
		m_gameStateVector->push_back(m_emptyState);
	}

	m_emptyState.frame = -1;
}

//GameState GameStateManager::GetInitialState()
//{
//	
//}

GameStateManager::~GameStateManager()
{
	
}

void GameStateManager::SaveState(GameState state)
{
	if (m_gameStateVector->back().frame < state.frame)
	{
		if (m_gameStateVector->size() >= m_maxStateVectorSize)
		{
			m_gameStateVector->erase(m_gameStateVector->begin());
		}
		m_gameStateVector->push_back(state);
		return;
	}


	for (std::vector<GameState>::reverse_iterator it = m_gameStateVector->rbegin(); it != m_gameStateVector->rend(); ++it)
	{
		if (state.frame == it->frame)
		{
			*it = state;
			return;
		}
	}

	std::cout << "Not saved state error" << std::endl;
}

GameState GameStateManager::GetState(int frame)
{
	for (std::vector<GameState>::reverse_iterator it = m_gameStateVector->rbegin(); it != m_gameStateVector->rend(); ++it)
	{
		if (frame == it->frame)
		{
			return *it;
		}
	}
	std::cout << "bad state" << std::endl;
	return m_emptyState;
}

void GameStateManager::TrimRolledbackStates(int frame)
{
	while (m_gameStateVector->back().frame > frame)
	{
		if (m_gameStateVector->back().frame == -1)
		{
			std::cout << "back frame -1" << std::endl;
			return;
		}
		m_gameStateVector->pop_back();
		//m_gameStateVector->insert(m_gameStateVector->begin(), m_emptyState);
	}
}


void GameStateManager::CreateNewGameState(GameState player1State, GameState player2State, GameState gameState)
{
	GameState newGameState;

	newGameState.frame = gameState.frame;

	if (player1State.player1Position.y == 0)
	{
		std::cout << "player 1 pos wrong" << std::endl;
	}

	newGameState.player1HitLanded = player1State.player1HitLanded;
	newGameState.player1Action = player1State.player1Action;
	newGameState.player1Anim= player1State.player1Anim;
	newGameState.player1ActionFrame = player1State.player1ActionFrame;
	newGameState.player1AnimFrame = player1State.player1AnimFrame;
	newGameState.player1CurrentInput = player1State.player1CurrentInput;
	newGameState.player1Direction = player1State.player1Direction;
	newGameState.player1Health = player1State.player1Health;
	newGameState.player1HitBy = player1State.player1HitBy;
	newGameState.player1IsBlocking = player1State.player1IsBlocking;
	newGameState.player1IsHitboxActive = player1State.player1IsHitboxActive;
	newGameState.player1JumpSpeed = player1State.player1JumpSpeed;
	newGameState.player1Position = player1State.player1Position;
	newGameState.player1SpritePosition = player1State.player1SpritePosition;
	newGameState.player1Pushback = player1State.player1Pushback;
	newGameState.player1PushbackFrame = player1State.player1PushbackFrame;
	newGameState.player1State = player1State.player1State;
	newGameState.player1StunFrames = player1State.player1StunFrames;


	newGameState.player2HitLanded = player2State.player2HitLanded;
	newGameState.player2Action = player2State.player2Action;
	newGameState.player2Anim = player2State.player2Anim;
	newGameState.player2ActionFrame = player2State.player2ActionFrame;
	newGameState.player2AnimFrame = player2State.player2AnimFrame;
	newGameState.player2CurrentInput = player2State.player2CurrentInput;
	newGameState.player2Direction = player2State.player2Direction;
	newGameState.player2Health = player2State.player2Health;
	newGameState.player2HitBy = player2State.player2HitBy;
	newGameState.player2IsBlocking = player2State.player2IsBlocking;
	newGameState.player2IsHitboxActive = player2State.player2IsHitboxActive;
	newGameState.player2JumpSpeed = player2State.player2JumpSpeed;
	newGameState.player2Position = player2State.player2Position;
	newGameState.player2SpritePosition = player2State.player2SpritePosition;
	newGameState.player2Pushback = player2State.player2Pushback;
	newGameState.player2PushbackFrame = player2State.player2PushbackFrame;
	newGameState.player2State = player2State.player2State;
	newGameState.player2StunFrames = player2State.player2StunFrames;

	newGameState.roundTimer = gameState.roundTimer;
	newGameState.framesInSecond = gameState.framesInSecond;

	SaveState(newGameState);
}
