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
		noInput.frameNumber = i;
		player1Inputs->push_back(noInput);
	}
	noInput.frameNumber = -1;
	m_currentFrame = 0;
}
//FIX ALL OF THIS


void Input::SetInput(FrameInput frameInput)
{
	//player1Inputs->insert(player1Inputs->at(frameInput.frameNumber), frameInput);
}

void Input::SetCurrentInput(int frame)
{
	while (player1Inputs->back().frameNumber > frame)
	{
		if (player1Inputs->back().frameNumber == -1)
		{
			return;
		}
		player1Inputs->pop_back();
		player1Inputs->insert(player1Inputs->begin(), noInput);
	}
}

//FrameInput Input::GetCurrentInput(int frame)
//{
//	return noInput;
//}


FrameInput Input::GetInput(int frameNo)
{
	for (std::vector<FrameInput>::reverse_iterator it = player1Inputs->rbegin(); it != player1Inputs->rend(); ++it)
	{
		if (it->frameNumber == frameNo)
		{
			return *it;
		}
	}
	return noInput;
}

void Input::UpdateInputs(int frameNo)
{
	FrameInput tempInput;
	tempInput.frameNumber = frameNo;
	

	for (auto &b: tempInput.inputs)
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
	
	if (player1Inputs->back().frameNumber < tempInput.frameNumber)
	{
		player1Inputs->erase(player1Inputs->begin());
		player1Inputs->push_back(tempInput);
		return;
	}

	for (std::vector<FrameInput>::reverse_iterator it = player1Inputs->rbegin(); it != player1Inputs->rend(); ++it)
	{
		if (tempInput.frameNumber == it->frameNumber)
		{
			*it = tempInput;
			return;
		}
	}
}

void Input::UpdateNoInputs(int frameNo)
{
	FrameInput tempInput = noInput;
	tempInput.frameNumber = frameNo;

	if (player1Inputs->back().frameNumber < tempInput.frameNumber)
	{
		player1Inputs->erase(player1Inputs->begin());
		player1Inputs->push_back(tempInput);
		return;
	}

	for (std::vector<FrameInput>::reverse_iterator it = player1Inputs->rbegin(); it != player1Inputs->rend(); ++it)
	{
		if (tempInput.frameNumber == it->frameNumber)
		{
			*it = tempInput;
			return;
		}
	}
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
