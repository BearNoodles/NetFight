#pragma once
#include "SFML/Graphics.hpp"

//Data relating to an actions
struct Action
{
	int damage;
	int blockstun, hitstun;
	int startupFrames, activeFrames, recoveryFrames, framesX, framesY, framesT;
	int blockPushback, hitPushback, pushbackFrames, selfHitPushback, selfBlockPushback, selfPushbackFrames;
	sf::Vector2f startupMove, activeMove, recoveryMove;
	sf::RectangleShape activeHitbox, startupHurtbox, activeHurtbox, recoveryHurtbox;
	int attackNumber;
};

//Data relating to an actions animation
struct Animation
{
	sf::Sprite* spriteSheet;
	int framesPerStep;
	int offsetX = 0;
	int offsetY = 0;

	int frames;
	int spriteWidth;
	int spriteHeight;

	bool loop = false;
};

//All possible states for the fighter class
enum PlayerState
{
	idle, attack1, attack2, hit, block, jumpU, jumpF, jumpB, jumpHit, walkF, walkB
};

//All action a fighter can perform and there animations
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
	Action jumpHit;
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
	Animation jumpHitAnim;
	Animation idleAnim;
};

class CharacterData
{
public:
	CharacterData();
	~CharacterData();

	CharacterStruct LoadCharacter1();
	
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
	sf::Texture m_kenJHitTexture;
	sf::Texture m_kenHitTexture;
	sf::Texture m_kenBlockTexture;

};

