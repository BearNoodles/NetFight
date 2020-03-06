#include "CharacterData.h"
#include <iostream>


CharacterData::CharacterData()
{
	
}

//TODO:
//Add hurtboxes for each move
CharacterStruct CharacterData::LoadCharacter1()
{
	m_characterStruct.attack1.damage = 50;
	m_characterStruct.attack1.startupFrames = 4;
	m_characterStruct.attack1.activeFrames = 3;
	m_characterStruct.attack1.recoveryFrames = 15;
	m_characterStruct.attack1.framesT = m_characterStruct.attack1.startupFrames + m_characterStruct.attack1.activeFrames + m_characterStruct.attack1.recoveryFrames;
	m_characterStruct.attack1.blockstun = 10;
	m_characterStruct.attack1.hitstun = 20;
	m_characterStruct.attack1.activeHitbox.setSize(sf::Vector2f(100, 50));
	m_characterStruct.attack1.activeHitbox.setPosition(sf::Vector2f(50, 50));
	m_characterStruct.attack1.hitPushback = 4;
	m_characterStruct.attack1.blockPushback = 10;
	m_characterStruct.attack1.pushbackFrames = 2;
	m_characterStruct.attack1.selfHitPushback = 4;
	m_characterStruct.attack1.selfBlockPushback = 10;
	m_characterStruct.attack1.selfPushbackFrames = 2;
	m_characterStruct.attack1.attackNumber = 1;

	m_characterStruct.attack2.damage = 200;
	m_characterStruct.attack2.startupFrames = 12;
	m_characterStruct.attack2.activeFrames = 2;
	m_characterStruct.attack2.recoveryFrames = 15;
	m_characterStruct.attack2.framesT = m_characterStruct.attack1.startupFrames + m_characterStruct.attack1.activeFrames + m_characterStruct.attack1.recoveryFrames;
	m_characterStruct.attack2.blockstun = 30;
	m_characterStruct.attack2.hitstun = 30;
	m_characterStruct.attack2.activeHitbox.setSize(sf::Vector2f(150, 50));
	m_characterStruct.attack2.activeHitbox.setPosition(sf::Vector2f(75, 150));
	m_characterStruct.attack2.hitPushback = 7;
	m_characterStruct.attack2.blockPushback = 20;
	m_characterStruct.attack2.pushbackFrames = 2;
	m_characterStruct.attack2.selfHitPushback = 4;
	m_characterStruct.attack2.selfBlockPushback = 10;
	m_characterStruct.attack2.selfPushbackFrames = 2;
	m_characterStruct.attack2.attackNumber = 2;

	m_characterStruct.idle.framesT = 1;
	m_characterStruct.block.framesT = 1;
	m_characterStruct.hit.framesT = 1;
	m_characterStruct.jump.framesT = 1;
	m_characterStruct.walk.framesT = 1;




	//Animation stuff
	if (!m_characterStruct.attackAnim1.texture.loadFromFile("image2.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}


	m_characterStruct.attackAnim1.spriteWidth = 4246;
	m_characterStruct.attackAnim1.spriteHeight = 171;
	m_characterStruct.attackAnim1.frames = m_characterStruct.attack1.framesT;
	m_characterStruct.attackAnim1.spriteSheet.setTexture(m_characterStruct.attackAnim1.texture);

	m_characterStruct.idleAnim.spriteWidth = 193;
	m_characterStruct.idleAnim.spriteHeight = 171;
	m_characterStruct.idleAnim.frames = 1;
	m_characterStruct.idleAnim.spriteSheet.setTexture(m_characterStruct.attackAnim1.texture);


	return m_characterStruct;
}

//CharacterStruct CharacterData::GetCharacterStruct()
//{
//	return m_characterStruct;
//}


CharacterData::~CharacterData()
{
}
