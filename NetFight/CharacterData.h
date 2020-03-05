#pragma once
#include "SFML/Graphics.hpp"

struct Action
{
	int damage;
	int blockstun, hitstun;
	int startupFrames, activeFrames, recoveryFrames, framesX, framesY, framesT;
	int blockPushback, hitPushback, pushbackFrames, selfHitPushback, selfBlockPushback, selfPushbackFrames;
	sf::Vector2f startupMove, activeMove, recoveryMove;
	sf::RectangleShape activeHitbox, startupHurtbox, activeHurtbox, recoveryHurtbox;
	//Texture2D txr;
	//SoundEffect sound;
};

enum PlayerState
{
	idle, attacking, hit, block, jump
};

struct CharacterStruct
{
	Action attack1;
	Action attack2;

	sf::Sprite spriteSheet;
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

	sf::Sprite spriteSheet1;

};

