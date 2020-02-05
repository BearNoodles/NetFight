#include "CharacterData.h"



CharacterData::CharacterData()
{
}

//TODO:
//Add hurtboxes for each move
void CharacterData::LoadCharacter1()
{
	m_characterStruct.attack1.damage = 50;
	m_characterStruct.attack1.startupFrames = 5;
	m_characterStruct.attack1.activeFrames = 2;
	m_characterStruct.attack1.recoveryFrames = 8;
	m_characterStruct.attack1.hitstun = 10;
	m_characterStruct.attack1.activeHitbox.setSize(sf::Vector2f(100, 50));
	m_characterStruct.attack1.activeHitbox.setPosition(sf::Vector2f(50, 50));
	m_characterStruct.attack1.hitPushback = 4;
	m_characterStruct.attack1.blockPushback = 10;
	m_characterStruct.attack1.pushbackFrames = 2;
	m_characterStruct.attack1.selfHitPushback = 4;
	m_characterStruct.attack1.selfBlockPushback = 10;
	m_characterStruct.attack1.selfPushbackFrames = 2;

	m_characterStruct.attack2.damage = 200;
	m_characterStruct.attack2.startupFrames = 12;
	m_characterStruct.attack2.activeFrames = 2;
	m_characterStruct.attack2.recoveryFrames = 15;
	m_characterStruct.attack2.hitstun = 30;
	m_characterStruct.attack2.activeHitbox.setSize(sf::Vector2f(150, 50));
	m_characterStruct.attack2.activeHitbox.setPosition(sf::Vector2f(75, 150));
	m_characterStruct.attack2.hitPushback = 7;
	m_characterStruct.attack2.blockPushback = 20;
	m_characterStruct.attack2.pushbackFrames = 2;
	m_characterStruct.attack2.selfHitPushback = 4;
	m_characterStruct.attack2.selfBlockPushback = 10;
	m_characterStruct.attack2.selfPushbackFrames = 2;


}

CharacterStruct CharacterData::GetCharacterStruct()
{
	return m_characterStruct;
}


CharacterData::~CharacterData()
{
}
