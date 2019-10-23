#pragma once
#include "SFML/Graphics.hpp"
#include "Input.h"
#include "CharacterData.h"

//struct fighterStruct
//{
//	sf::RectangleShape m_hurtBox;
//	sf::RectangleShape m_hitBoxes[5];
//
//	sf::Vector2f m_position;
//};
class Fighter
{
public:
	Fighter(sf::Vector2f position, int playerNumber, int screenWidth);
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

	float GetHealth();

protected:

	sf::RectangleShape m_hurtbox;
	sf::RectangleShape* m_activeHitbox;
	//sf::RectangleShape m_hitboxes[5];

	sf::RectangleShape m_healthBar;

	float m_maxHealth;
	float m_currentHealth;

	int m_walkSpeed;

	sf::Vector2f m_hitboxPosition;

	bool m_isHitboxActive;

	sf::Vector2f m_position;

	int m_playerID;

	FrameInput m_currentInput;

	//MAKE STATIC?
	CharacterData m_characterData;
	CharacterStruct m_characterActions;

	Action m_currentAction;

	enum State
	{
		idle, attacking, hit, block, jump
	};

	Action m_hitBy;

	int m_actionFrame;
	int m_stunFrames;
	int m_pushback;
	State m_attackState;
	void ChangeState(State attackState);

	void StartJump(int direction);
	void UpdateJump();
	sf::Vector2i m_jumpSpeed;
	sf::Vector2i m_initialJumpSpeed;

	int m_floorPosition;

	int m_direction;

	int m_screenWidth;

	bool m_hitLanded;

	int m_pushbackFrame;

	bool m_isBlocking;
};

