#pragma once
#include "CharacterData.h"
#include "Input.h"
#include "SFML/Graphics.hpp"

struct GameState
{
	int frame;

	int player1Health;
	int player2Health;

	sf::Vector2f player1Position;
	sf::Vector2f player2Position;

	Action player1Action;
	Action player2Action;

	int player1ActionFrame;
	int player2ActionFrame;

	PlayerState player1State;
	PlayerState player2State;

	bool player1IsHitboxActive;
	bool player2IsHitboxActive;

	FrameInput player1CurrentInput;
	FrameInput player2CurrentInput;

	Action player1HitBy;
	Action player2HitBy;

	int player1StunFrames;
	int player2StunFrames;

	int player1Pushback;
	int player2Pushback;

	int player1PushbackFrame;
	int player2PushbackFrame;

	sf::Vector2i player1JumpSpeed;
	sf::Vector2i player2JumpSpeed;

	int player1Direction;
	int player2Direction;

	bool player1IsBlocking;
	bool player2IsBlocking;

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

	void CreateNewGameState(GameState player1State, GameState player2State, GameState gameState);

protected:

	std::vector<GameState>* m_gameStateVector;

	GameState m_emptyState;

};

