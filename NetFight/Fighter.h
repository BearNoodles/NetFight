#pragma once
#include "SFML/Graphics.hpp"

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

protected:

	sf::RectangleShape m_hurtBox;
	//sf::RectangleShape m_hitBoxes[5];

	sf::Vector2f m_position;




};

