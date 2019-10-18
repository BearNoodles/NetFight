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

protected:

	sf::RectangleShape m_hurtbox;
	sf::RectangleShape* m_activeHitbox;
	//sf::RectangleShape m_hitboxes[5];

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
		idle, attacking, hit
	};

	int m_actionFrame;
	int m_hitStunFrames;
	State m_attackState;
	void ChangeState(State attackState);

	int m_direction;

	int m_screenWidth;
};

