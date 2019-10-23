#pragma once
#include "CharacterData.h"

struct GameState
{
	int frame;

	int player1Health;
	int player2Health;

	int player1Position;
	int player2Position;

	Action player1Action;
	Action player2Action;

	int player1ActionFrame;
	int player2ActionFrame;

	float time;
};
class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void SaveState(GameState state);
	GameState GetState(int frame);
	void LoadState(int frame);

protected:

	std::vector<GameState>* m_gameStateVector;

	GameState m_emptyState;

};

