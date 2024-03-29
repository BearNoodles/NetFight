#pragma once
#include "CharacterData.h"
#include "Input.h"
#include "SFML/Graphics.hpp"

//Everything included in a gamestate to be saved/loaded
struct GameState
{
	int frame;

	bool player1HitLanded;
	bool player2HitLanded;

	int player1Health;
	int player2Health;

	sf::Vector2i player1Position;
	sf::Vector2i player2Position;

	sf::Vector2i player1SpritePosition;
	sf::Vector2i player2SpritePosition;

	Action player1Action;
	Action player2Action;

	Animation player1Anim;
	Animation player2Anim;

	int player1ActionFrame;
	int player2ActionFrame;

	int player1AnimFrame;
	int player2AnimFrame;

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

	int framesInSecond;
	int roundTimer;

	int localPlayerHandle;
};
class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void SaveState(GameState state);
	GameState GetState(int frame);
	void TrimRolledbackStates(int frame);

	void CreateNewGameState(GameState player1State, GameState player2State, GameState gameState);

	void Reset();

	std::vector<GameState>* GetAllInputs();

protected:

	std::vector<GameState>* m_gameStateVector;

	GameState m_emptyState;

	int m_maxStateVectorSize;
};

