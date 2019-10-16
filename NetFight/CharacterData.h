#pragma once
#include "SFML/Graphics.hpp"

struct Action
{
	int blockstun, hitstun;
	int startupFrames, activeFrames, recoveryFrames, framesX, framesY, framesT;
	sf::Vector2f startupMove, activeMove, recoveryMove;
	sf::RectangleShape activeHitbox, startupHurtbox, activeHurtbox, recoveryHurtbox;
	//Texture2D txr;
	//SoundEffect sound;
};

struct CharacterStruct
{
	Action attack1;
	Action attack2;
};

class CharacterData
{
public:
	CharacterData();
	~CharacterData();

	void LoadCharacter1();

	CharacterStruct m_characterStruct;
};

