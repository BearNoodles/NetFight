#include "Input.h"



Input::Input()
{
	for (bool b : noInput.inputs)
	{
		b = false;
	}
	noInput.frameNumber = -1;
	player1Inputs = new std::vector<FrameInput>();
	for (int i = 0; i < 100; i++)
	{
		player1Inputs->push_back(noInput);
	}
	m_currentFrame = 0;
}
//FIX ALL OF THIS


void Input::SetInput(FrameInput frameInput)
{
	//player1Inputs->insert(player1Inputs->at(frameInput.frameNumber), frameInput);
}

FrameInput Input::GetInput(int frameNo)
{
	return player1Inputs->back();
}

void Input::UpdateInputs(int frameNo)
{
	FrameInput tempInput;
	tempInput.frameNumber = frameNo;
	
	player1Inputs->erase(player1Inputs->begin());
	

	for (bool b: tempInput.inputs)
	{
		b = false;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		tempInput.inputs[1] = true;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		tempInput.inputs[0] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		tempInput.inputs[2] = false;
		tempInput.inputs[3] = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		tempInput.inputs[2] = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		tempInput.inputs[3] = true;
	}

	player1Inputs->push_back(tempInput);
}

void Input::UpdateAll()
{
	
}

void Input::SetCurrentFrame(int frame)
{
	m_currentFrame = frame;
}


Input::~Input()
{
}
