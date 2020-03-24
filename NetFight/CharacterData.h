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
	sf::Sprite* spriteSheet;
	int framesPerStep;
	int offsetX = 0;
	int offsetY = 0;
	//sf::Texture texture;
	//std::vector<sf::Vector2i> animRect;

	int frames;
	int spriteWidth;
	int spriteHeight;

	bool loop = false;
};

enum PlayerState
{
	idle, attack1, attack2, hit, block, jumpU, jumpF, jumpB, walkF, walkB
};

struct CharacterStruct
{
	Action attack1;
	Action attack2;
	Action idle;
	Action hit;
	Action block;
	Action jumpU;
	Action jumpF;
	Action jumpB;
	Action walkF;
	Action walkB;

	int preJumpFrames;

	Animation attackAnim1;
	Animation attackAnim2;
	Animation walkFAnim;
	Animation walkBAnim;
	Animation hitAnim;
	Animation blockAnim;
	Animation jumpUAnim;
	Animation jumpFAnim;
	Animation jumpBAnim;
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

	sf::Texture m_kenAttTexture1;
	sf::Texture m_kenAttTexture2;
	sf::Texture m_kenIdleTexture;
	sf::Texture m_kenWalkFTexture;
	sf::Texture m_kenWalkBTexture;
	sf::Texture m_kenJUTexture;
	sf::Texture m_kenJFTexture;
	sf::Texture m_kenJBTexture;
	sf::Texture m_kenHitTexture;
	sf::Texture m_kenBlockTexture;

};

