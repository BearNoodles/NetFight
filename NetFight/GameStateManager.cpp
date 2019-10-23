#include "GameStateManager.h"



GameStateManager::GameStateManager()
{
	m_emptyState.frame = -1; 
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
