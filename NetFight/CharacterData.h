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
	int attackNumber;
};

struct Animation
{
	sf::Sprite spriteSheet;
	sf::Texture texture;
	//std::vector<sf::Vector2i> animRect;

	int frames;
	int spriteWidth;
	int spriteHeight;
};

enum PlayerState
{
	idle, attack1, attack2, hit, block, jump
};

struct CharacterStruct
{
	Action attack1;
	Action attack2;
	Action idle;
	Action hit;
	Action block;
	Action jump;
	Action walk;

	Animation attackAnim1;
	Animation attackAnim2;
	Animation walkAnim;
	Animation hitAnim;
	Animation blockAnim;
	Animation jumpAnim;
	Animation idleAnim;
};

class CharacterData
{
public:
	CharacterData();
	~CharacterData();

	CharacterStruct LoadCharacter1();

	//CharacterStruct GetCharacterStruct();
	
protected:
	CharacterStruct m_characterStruct;

};

