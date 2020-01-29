#include "Input.h"

Input::Input()
{
	for (bool b : noInput.inputs)
	{
		b = false;
	}
	noInput.frameNumber = -1;
	opponentInputs = new std::vector<FrameInput>();
	localInputs = new std::vector<FrameInput>();
	for (int i = 0; i < 10; i++)
	{
		noInput.frameNumber = i;
		opponentInputs->push_back(noInput);
		localInputs->push_back(noInput);
	}
	noInput.frameNumber = -1;
	m_currentFrame = 0;
}
//FIX ALL OF THIS

//AAALLL OF IT


FrameInput Input::ReadLocalInput(int frameNo)
{
	FrameInput tempInput;
	tempInput.frameNumber = frameNo;


	for (auto& b : tempInput.inputs)
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
	
	return tempInput;
}

void Input::SetInput(FrameInput frameInput)
{
	//player1Inputs->insert(player1Inputs->at(frameInput.frameNumber), frameInput);
}

//void Input::SetOpponentInput(int frame)
//{
//	while (opponentInputs->back().frameNumber > frame)
//	{
//		if (opponentInputs->back().frameNumber == -1)
//		{
//			return;
//		}
//		opponentInputs->pop_back();
//		opponentInputs->insert(opponentInputs->begin(), noInput);
//	}
//}

//FrameInput Input::GetCurrentInput(int frame)
//{
//	return noInput;
//}


FrameInput Input::GetOpponentInput(int frameNo)
{
	for (std::vector<FrameInput>::reverse_iterator it = opponentInputs->rbegin(); it != opponentInputs->rend(); ++it)
	{
		if (it->frameNumber == frameNo)
		{
			return *it;
		}
	}
	return noInput;
}

FrameInput Input::GetLocalInput(int frameNo)
{
	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (it->frameNumber == frameNo)
		{
			return *it;
		}
	}
	return noInput;
}

FrameInput Input::GetNoInput(int frameNo)
{
	FrameInput tempInput = noInput;

	tempInput.frameNumber = frameNo;

	return tempInput;
}

void Input::SetOpponentInput(FrameInput input)
{
	if (opponentInputs->back().frameNumber < input.frameNumber)
	{
		opponentInputs->erase(opponentInputs->begin());
		opponentInputs->push_back(input);
		return;
	}

	for (std::vector<FrameInput>::reverse_iterator it = opponentInputs->rbegin(); it != opponentInputs->rend(); ++it)
	{
		if (input.frameNumber == it->frameNumber)
		{
			*it = input;
			return;
		}
	}
}

void Input::SetLocalInput(FrameInput input)
{
	if (localInputs->back().frameNumber < input.frameNumber)
	{
		localInputs->erase(localInputs->begin());
		localInputs->push_back(input);
		return;
	}

	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (input.frameNumber == it->frameNumber)
		{
			*it = input;
			return;
		}
	}
}


std::vector<FrameInput>* Input::GetAllInputs(int player)
{
	if (player == 1)
	{
		return opponentInputs;
	}
	else
	{
		return localInputs;
	}
}
//void Input::UpdateNoInputs(int frameNo)
//{
//	FrameInput tempInput = noInput;
//	tempInput.frameNumber = frameNo;
//
//	if (player1Inputs->back().frameNumber < tempInput.frameNumber)
//	{
//		player1Inputs->erase(player1Inputs->begin());
//		player1Inputs->push_back(tempInput);
//		return;
//	}
//
//	for (std::vector<FrameInput>::reverse_iterator it = player1Inputs->rbegin(); it != player1Inputs->rend(); ++it)
//	{
//		if (tempInput.frameNumber == it->frameNumber)
//		{
//			*it = tempInput;
//			return;
//		}
//	}
//}

void Input::UpdateAll()
{
	
}

void Input::SetCurrentFrame(int frame)
{
	m_currentFrame = frame;
}

bool Input::BothInputsReady(int frame)
{
	if (GetOpponentInput(frame).frameNumber != -1)
	{
		return true;
	}

	return false;
}


Input::~Input()
{
}
