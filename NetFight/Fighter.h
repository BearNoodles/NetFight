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
	Fighter(sf::Vector2f position);
	~Fighter();

	void UpdateFrame();

	sf::RectangleShape GetHurtbox();

	void CreateHitbox();

	sf::RectangleShape GetActiveHitbox();

	void SetInput(FrameInput input);

	bool IsHitboxActive();

protected:

	sf::RectangleShape m_hurtbox;
	sf::RectangleShape* m_activeHitbox;
	//sf::RectangleShape m_hitboxes[5];

	sf::Vector2f m_hitboxPosition;

	bool m_isHitboxActive;

	sf::Vector2f m_position;

	int m_playerID;

	FrameInput m_currentInput;

	CharacterData m_characterData;

	enum State
	{
		idle, startup, active, recovery
	};

	int m_currentActionFrame;
	State m_attackState;
	void ChangeState(State attackState);

};

