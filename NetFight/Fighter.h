#pragma once
#include "SFML/Graphics.hpp"
#include "Input.h"

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

	void SetInput(FrameInput input);

protected:

	sf::RectangleShape m_hurtBox;
	//sf::RectangleShape m_hitBoxes[5];

	sf::Vector2f m_position;

	int m_playerID;

	FrameInput m_currentInput;




};

