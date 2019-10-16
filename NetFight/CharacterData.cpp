#include "CharacterData.h"



CharacterData::CharacterData()
{
}

void CharacterData::LoadCharacter1()
{
	m_characterStruct.attack1.startupFrames = 10;
	m_characterStruct.attack1.activeFrames = 2;
	m_characterStruct.attack1.recoveryFrames = 10;
}


CharacterData::~CharacterData()
{
}
