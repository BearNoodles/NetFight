#pragma once
#include "SFML/Graphics.hpp"
#include "Input.h"
#include "CharacterData.h"
#include "GameStateManager.h"

//struct fighterStruct
//{
//	sf::RectangleShape m_hurtBox;
//	sf::RectangleShape m_hitBoxes[5];
//
//	sf::Vector2f m_position;
//};

//enum State
//{
//	idle, attacking, hit, block, jump
//};

class Fighter
{
public:
	Fighter(sf::Vector2i position, int playerNumber, int screenWidth, int floorHeight);
	~Fighter();

	void UpdateFrame();

	sf::RectangleShape GetHurtbox();

	void CreateHitbox();

	sf::RectangleShape GetActiveHitbox();

	Action GetCurrentAction();

	void SetInput(FrameInput input);

	bool IsHitboxActive();

	bool CheckForHit(sf::RectangleShape* opponentHitbox);

	void HandleCollision(Action opponentAttack);

	void RemoveActiveHitbox();

	void HitLanded();

	bool IsCornered();

	bool CheckPushing(sf::RectangleShape opponentHurtbox);

	void WalkPush();

	int GetHealth();

	void SetFighterState(GameState gameState);
	GameState GetFighterState();

	sf::Sprite* GetAnimationFrame();

	void SetCharacterData(CharacterStruct data);

	int GetDirection();

protected:

	sf::RectangleShape m_hurtbox;
	sf::RectangleShape* m_activeHitbox;
	//sf::RectangleShape m_hitboxes[5];

	sf::RectangleShape m_healthBar;

	int m_maxHealth;
	int m_currentHealth;

	int m_walkSpeed;

	sf::Vector2i m_hitboxPosition;

	bool m_isHitboxActive;

	sf::Vector2i m_position;
	sf::Vector2i m_spritePosition;

	sf::Vector2i m_hurtboxOffset;

	int m_playerID;

	FrameInput m_currentInput;

	//MAKE STATIC?
	CharacterData m_characterData;
	CharacterStruct m_characterActions;


	Action m_currentAction;

	Animation m_currentAnim;
	sf::IntRect m_animRect;

	void SetAnimRect();

	/*enum State
	{
		idle, attacking, hit, block, jump
	};*/

	Action m_hitBy;

	int m_actionFrame;
	int m_stunFrames;
	int m_pushback;
	PlayerState m_playerState;
	void ChangeState(PlayerState playerState);

	void StartJump(int direction);
	void UpdateJump();
	sf::Vector2i m_jumpSpeed;
	sf::Vector2i m_initialJumpSpeed;

	int m_jumpSpeedLossRate;

	int m_floorPosition;
	int m_landPosition;

	int m_direction;

	int m_screenWidth;

	bool m_hitLanded;

	int m_pushbackFrame;

	bool m_isBlocking;

	sf::Sprite* m_spriteSheet;

	sf::Vector2f offsetVec;
};

