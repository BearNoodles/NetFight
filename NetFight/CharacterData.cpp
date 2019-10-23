#include "CharacterData.h"



CharacterData::CharacterData()
{
}

void CharacterData::LoadCharacter1()
{
	m_characterStruct.attack1.damage = 100;
	m_characterStruct.attack1.startupFrames = 10;
	m_characterStruct.attack1.activeFrames = 2 ;
	m_characterStruct.attack1.recoveryFrames = 10;
	m_characterStruct.attack1.hitstun = 10;
	m_characterStruct.attack1.activeHitbox.setSize(sf::Vector2f(100, 50));
	m_characterStruct.attack1.activeHitbox.setPosition(sf::Vector2f(50, 50));
	m_characterStruct.attack1.hitPushback = 1;
	m_characterStruct.attack1.blockPushback = 3;
	m_characterStruct.attack1.pushbackFames = 2;
	m_characterStruct.attack1.selfHitPushback = 1;
	m_characterStruct.attack1.selfBlockPushback = 3;
	m_characterStruct.attack1.selfPushbackFrames = 2;


}

CharacterStruct CharacterData::GetCharacterStruct()
{
	return m_characterStruct;
}


CharacterData::~CharacterData()
{
}
