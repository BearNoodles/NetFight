#include "CharacterData.h"
#include <iostream>


CharacterData::CharacterData()
{
	
}

//TODO:
//Add different hurtboxes for each move

//Loads all of character 1's datat
CharacterStruct CharacterData::LoadCharacter1()
{
	m_characterStruct.attack1.damage = 50;
	m_characterStruct.attack1.startupFrames = 3;
	m_characterStruct.attack1.activeFrames = 1;
	m_characterStruct.attack1.recoveryFrames = 2;
	m_characterStruct.attack1.framesT = m_characterStruct.attack1.startupFrames + m_characterStruct.attack1.activeFrames + m_characterStruct.attack1.recoveryFrames;
	m_characterStruct.attack1.blockstun = 8;
	m_characterStruct.attack1.hitstun = 12;
	m_characterStruct.attack1.activeHitbox.setSize(sf::Vector2f(100, 50));
	m_characterStruct.attack1.activeHitbox.setPosition(sf::Vector2f(100, 0));
	m_characterStruct.attack1.hitPushback = 4;
	m_characterStruct.attack1.blockPushback = 10;
	m_characterStruct.attack1.pushbackFrames = 2;
	m_characterStruct.attack1.selfHitPushback = 4;
	m_characterStruct.attack1.selfBlockPushback = 10;
	m_characterStruct.attack1.selfPushbackFrames = 2;
	m_characterStruct.attack1.hitStop = 5;
	m_characterStruct.attack1.blockStop = 5;
	m_characterStruct.attack1.attackNumber = 1;

	m_characterStruct.attack2.damage = 200;
	m_characterStruct.attack2.startupFrames = 6;
	m_characterStruct.attack2.activeFrames = 1;
	m_characterStruct.attack2.recoveryFrames = 5;
	m_characterStruct.attack2.framesT = m_characterStruct.attack2.startupFrames + m_characterStruct.attack2.activeFrames + m_characterStruct.attack2.recoveryFrames;
	m_characterStruct.attack2.blockstun = 30;
	m_characterStruct.attack2.hitstun = 30;
	m_characterStruct.attack2.activeHitbox.setSize(sf::Vector2f(220, 50));
	m_characterStruct.attack2.activeHitbox.setPosition(sf::Vector2f(75, 50));
	m_characterStruct.attack2.hitPushback = 7;
	m_characterStruct.attack2.blockPushback = 20;
	m_characterStruct.attack2.pushbackFrames = 2;
	m_characterStruct.attack2.selfHitPushback = 4;
	m_characterStruct.attack2.selfBlockPushback = 10;
	m_characterStruct.attack2.selfPushbackFrames = 2;
	m_characterStruct.attack2.hitStop = 10;
	m_characterStruct.attack2.blockStop = 10;
	m_characterStruct.attack2.attackNumber = 2;

	//Actions with no attack or hitbox data can just use framesT
	m_characterStruct.idle.framesT = 10;
	m_characterStruct.block.framesT = 1;
	m_characterStruct.hit.framesT = 3;
	m_characterStruct.block.framesT = 4;
	m_characterStruct.jumpU.framesT = 21;
	m_characterStruct.jumpF.framesT = 22;
	m_characterStruct.jumpB.framesT = 22;
	m_characterStruct.jumpHit.framesT = 3;
	m_characterStruct.walkF.framesT = 11;
	m_characterStruct.walkB.framesT = 11;
	

	m_characterStruct.preJumpFrames = 4;


	//Load animation spritesheets
	if (!m_kenAttTexture1.loadFromFile("images/kenJab.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenAttTexture2.loadFromFile("images/kenHK2.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenIdleTexture.loadFromFile("images/kenIdle.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenWalkFTexture.loadFromFile("images/kenWalkF.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenWalkBTexture.loadFromFile("images/kenWalkB.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenJUTexture.loadFromFile("images/kenJU.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenJFTexture.loadFromFile("images/kenJF.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenJBTexture.loadFromFile("images/kenJB.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenJHitTexture.loadFromFile("images/kenHit.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenHitTexture.loadFromFile("images/kenHit.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}
	if (!m_kenBlockTexture.loadFromFile("images/kenBlock.png"))
	{
		// error...
		std::cout << "error loading image.png" << std::endl;
	}


	m_characterStruct.attackAnim1.spriteSheet = new sf::Sprite();
	m_characterStruct.attackAnim2.spriteSheet = new sf::Sprite();
	m_characterStruct.idleAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.walkFAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.walkBAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.jumpUAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.jumpFAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.jumpBAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.jumpHitAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.hitAnim.spriteSheet = new sf::Sprite();
	m_characterStruct.blockAnim.spriteSheet = new sf::Sprite();


	m_characterStruct.attackAnim1.spriteWidth = 1200;
	m_characterStruct.attackAnim1.spriteHeight = 218;
	m_characterStruct.attackAnim1.framesPerStep = 3;
	m_characterStruct.attackAnim1.loop = false;

	m_characterStruct.attack1.startupFrames *= m_characterStruct.attackAnim1.framesPerStep;
	m_characterStruct.attack1.activeFrames *= m_characterStruct.attackAnim1.framesPerStep;
	m_characterStruct.attack1.recoveryFrames *= m_characterStruct.attackAnim1.framesPerStep;
	m_characterStruct.attack1.framesT *= m_characterStruct.attackAnim1.framesPerStep;

	m_characterStruct.attackAnim1.spriteSheet->setTexture(m_kenAttTexture1);

	m_characterStruct.attackAnim2.spriteWidth = 4488;
	m_characterStruct.attackAnim2.spriteHeight = 242;
	m_characterStruct.attackAnim2.framesPerStep = 3;
	m_characterStruct.attackAnim2.loop = false;

	m_characterStruct.attack2.startupFrames *= m_characterStruct.attackAnim2.framesPerStep;
	m_characterStruct.attack2.activeFrames *= m_characterStruct.attackAnim2.framesPerStep;
	m_characterStruct.attack2.recoveryFrames *= m_characterStruct.attackAnim2.framesPerStep;
	m_characterStruct.attack2.framesT *= m_characterStruct.attackAnim2.framesPerStep;

	m_characterStruct.attackAnim2.offsetX = -50;
	m_characterStruct.attackAnim2.offsetY = -20;
	m_characterStruct.attackAnim2.spriteSheet->setTexture(m_kenAttTexture2);


	m_characterStruct.idleAnim.spriteWidth = 1560;
	m_characterStruct.idleAnim.spriteHeight = 222;
	m_characterStruct.idleAnim.framesPerStep = 3;
	m_characterStruct.idleAnim.loop = true;

	m_characterStruct.idle.framesT *= m_characterStruct.idleAnim.framesPerStep;

	m_characterStruct.idleAnim.spriteSheet->setTexture(m_kenIdleTexture);


	m_characterStruct.walkFAnim.spriteWidth = 2442;
	m_characterStruct.walkFAnim.spriteHeight = 226;
	m_characterStruct.walkFAnim.framesPerStep = 4;
	m_characterStruct.walkFAnim.loop = true;

	m_characterStruct.walkF.framesT *= m_characterStruct.walkFAnim.framesPerStep;

	m_characterStruct.walkFAnim.offsetX = -40;
	m_characterStruct.walkFAnim.spriteSheet->setTexture(m_kenWalkFTexture);


	m_characterStruct.walkBAnim.spriteWidth = 2464;
	m_characterStruct.walkBAnim.spriteHeight = 226;
	m_characterStruct.walkBAnim.framesPerStep = 4;
	m_characterStruct.walkBAnim.loop = true;

	m_characterStruct.walkB.framesT *= m_characterStruct.walkBAnim.framesPerStep;
	m_characterStruct.walkBAnim.offsetX = -40;

	m_characterStruct.walkBAnim.spriteSheet->setTexture(m_kenWalkBTexture);


	m_characterStruct.jumpUAnim.spriteWidth = 3612;
	m_characterStruct.jumpUAnim.spriteHeight = 380;
	m_characterStruct.jumpUAnim.framesPerStep = 3;
	m_characterStruct.jumpUAnim.loop = false;

	m_characterStruct.jumpU.framesT *= m_characterStruct.jumpUAnim.framesPerStep;
	m_characterStruct.jumpUAnim.offsetY = -158;

	//m_characterStruct.jumpUAnim.spriteSheet->setPosition(sf::Vector2f(0, m_characterStruct.jumpUAnim.offsetY));
	m_characterStruct.jumpUAnim.spriteSheet->setTexture(m_kenJUTexture);


	m_characterStruct.jumpFAnim.spriteWidth = 9680;
	m_characterStruct.jumpFAnim.spriteHeight = 360;
	m_characterStruct.jumpFAnim.framesPerStep = 3;
	m_characterStruct.jumpFAnim.loop = false;

	m_characterStruct.jumpF.framesT *= m_characterStruct.jumpFAnim.framesPerStep;

	m_characterStruct.jumpFAnim.offsetY = -138;
	m_characterStruct.jumpFAnim.spriteSheet->setTexture(m_kenJFTexture);


	m_characterStruct.jumpBAnim.spriteWidth = 10208;
	m_characterStruct.jumpBAnim.spriteHeight = 380;
	m_characterStruct.jumpBAnim.framesPerStep = 3;
	m_characterStruct.jumpBAnim.loop = false;

	m_characterStruct.jumpB.framesT *= m_characterStruct.jumpBAnim.framesPerStep;

	m_characterStruct.jumpBAnim.offsetX = -304;
	m_characterStruct.jumpBAnim.offsetY = -158;
	m_characterStruct.jumpBAnim.spriteSheet->setTexture(m_kenJBTexture);

	m_characterStruct.jumpHitAnim.spriteWidth = 852;
	m_characterStruct.jumpHitAnim.spriteHeight = 220;
	m_characterStruct.jumpHitAnim.framesPerStep = 3;
	m_characterStruct.jumpHitAnim.loop = false;

	m_characterStruct.jumpHit.framesT *= m_characterStruct.jumpHitAnim.framesPerStep;

	m_characterStruct.jumpHitAnim.offsetX = -100;
	m_characterStruct.jumpHitAnim.offsetY = 0;
	m_characterStruct.jumpHitAnim.spriteSheet->setTexture(m_kenJHitTexture);


	m_characterStruct.hitAnim.spriteWidth = 852;
	m_characterStruct.hitAnim.spriteHeight = 220;
	m_characterStruct.hitAnim.framesPerStep = 3;
	m_characterStruct.hitAnim.loop = false;

	m_characterStruct.hit.framesT *= m_characterStruct.hitAnim.framesPerStep;

	m_characterStruct.hitAnim.offsetX = -100;
	m_characterStruct.hitAnim.offsetY = 0;
	m_characterStruct.hitAnim.spriteSheet->setTexture(m_kenHitTexture);


	m_characterStruct.blockAnim.spriteWidth = 624;
	m_characterStruct.blockAnim.spriteHeight = 210;
	m_characterStruct.blockAnim.framesPerStep = 3;
	m_characterStruct.blockAnim.loop = false;

	m_characterStruct.block.framesT *= m_characterStruct.blockAnim.framesPerStep;

	m_characterStruct.blockAnim.offsetX = 0;
	m_characterStruct.blockAnim.offsetY = 0;
	m_characterStruct.blockAnim.spriteSheet->setTexture(m_kenBlockTexture);


	

	return m_characterStruct;
}



CharacterData::~CharacterData()
{
}
