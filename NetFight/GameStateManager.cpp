#include "GameStateManager.h"



GameStateManager::GameStateManager()
{
	m_gameStateVector = new std::vector<GameState>();
	m_emptyState.frame = -1; 
	m_gameStateVector->push_back(m_emptyState);
}


GameStateManager::~GameStateManager()
{
}

void GameStateManager::SaveState(GameState state)
{
	if (m_gameStateVector->back().frame < state.frame)
	{
		m_gameStateVector->erase(m_gameStateVector->begin());
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
	return m_emptyState;
}

void GameStateManager::CreateNewGameState(GameState player1State, GameState player2State, GameState gameState)
{
	GameState newGameState;

	newGameState.frame = gameState.frame;

	newGameState.player1Action = player1State.player1Action;
	newGameState.player1ActionFrame = player1State.player1ActionFrame;
	newGameState.player1CurrentInput = player1State.player1CurrentInput;
	newGameState.player1Direction = player1State.player1Direction;
	newGameState.player1Health = player1State.player1Health;
	newGameState.player1HitBy = player1State.player1HitBy;
	newGameState.player1IsBlocking = player1State.player1IsBlocking;
	newGameState.player1IsHitboxActive = player1State.player1IsHitboxActive;
	newGameState.player1JumpSpeed = player1State.player1JumpSpeed;
	newGameState.player1Position = player1State.player1Position;
	newGameState.player1Pushback = player1State.player1Pushback;
	newGameState.player1PushbackFrame = player1State.player1PushbackFrame;
	newGameState.player1State = player1State.player1State;
	newGameState.player1StunFrames = player1State.player1StunFrames;

	newGameState.player2Action = player2State.player2Action;
	newGameState.player2ActionFrame = player2State.player2ActionFrame;
	newGameState.player2CurrentInput = player2State.player2CurrentInput;
	newGameState.player2Direction = player2State.player2Direction;
	newGameState.player2Health = player2State.player2Health;
	newGameState.player2HitBy = player2State.player2HitBy;
	newGameState.player2IsBlocking = player2State.player2IsBlocking;
	newGameState.player2IsHitboxActive = player2State.player2IsHitboxActive;
	newGameState.player2JumpSpeed = player2State.player2JumpSpeed;
	newGameState.player2Position = player2State.player2Position;
	newGameState.player2Pushback = player2State.player2Pushback;
	newGameState.player2PushbackFrame = player2State.player2PushbackFrame;
	newGameState.player2State = player2State.player2State;
	newGameState.player2StunFrames = player2State.player2StunFrames;

	newGameState.time = gameState.time;

	SaveState(gameState);
}
