#pragma once
#include "SFML/Graphics.hpp"

struct Action
{
	int damage;
	int blockstun, hitstun;
	int startupFrames, activeFrames, recoveryFrames, framesX, framesY, framesT;
	int blockPushback, hitPushback, pushbackFames, selfHitPushback, selfBlockPushback, selfPushbackFrames;
	sf::Vector2f startupMove, activeMove, recoveryMove;
	sf::RectangleShape activeHitbox, startupHurtbox, activeHurtbox, recoveryHurtbox;
	//Texture2D txr;
	//SoundEffect sound;
};

enum State
{
	idle, attacking, hit, block, jump
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

	CharacterStruct GetCharacterStruct();
	
protected:
	CharacterStruct m_characterStruct;
};

