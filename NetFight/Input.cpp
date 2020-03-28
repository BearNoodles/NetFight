#include "Input.h"

Input::Input()
{
	opponentInputs = new std::vector<FrameInput>();
	localInputs = new std::vector<FrameInput>();
	
	Init();
}

void Input::Init()
{
	for (bool b : noInput.inputs)
	{
		b = false;
	}
	noInput.frameNumber = -1;
	for (int i = 0; i < 100; i++)
	{
		noInput.frameNumber = i;
		noInput.set = true;
		opponentInputs->push_back(noInput);
		localInputs->push_back(noInput);
	}
	noInput.frameNumber = -1;
	m_currentFrame = 0;

	deadZone = 20;
}

void Input::Reset()
{
	opponentInputs->clear();
	localInputs->clear();

	Init();
}

FrameInput Input::ReadLocalInput(int frameNo)
{
	FrameInput tempInput;
	tempInput.frameNumber = frameNo;


	for (auto& b : tempInput.inputs)
	{
		b = false;
	}

	std::cout << "X xis: " << sf::Joystick::getAxisPosition(0, sf::Joystick::X) << std::endl;
	std::cout << "Y xis: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Y) << std::endl;
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > deadZone)
	{
		tempInput.inputs[1] = true;
	}

	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -deadZone)
	{
		tempInput.inputs[0] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		tempInput.inputs[2] = false;
		tempInput.inputs[3] = false;
	}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -deadZone)
	{
		tempInput.inputs[2] = true;
	}
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > deadZone)
	{
		tempInput.inputs[3] = true;
	}

	if (sf::Joystick::isButtonPressed(0, 2))
	{
		tempInput.inputs[4] = true;
	}

	if (sf::Joystick::isButtonPressed(0, 3))
	{
		tempInput.inputs[5] = true;
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

	std::cout << "THIS SHOULD NEVER HAPPEN" << std::endl;

	FrameInput temp = noInput;
	temp.frameNumber = frameNo;
	return temp;
}

FrameInput Input::GetLocalInput(int frameNo)
{
	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (it->frameNumber == frameNo)
		{
			//it->set = true;
			if (!it->set)
			{
				return *it;
			}
			return *it;
		}
	}
	FrameInput temp = noInput;
	temp.frameNumber = frameNo;

	temp.set = true;

	InsertLocalInput(temp);
	return temp;
}

FrameInput Input::GetNoInput(int frameNo)
{
	FrameInput tempInput = noInput;

	tempInput.frameNumber = frameNo;

	tempInput.set = true;

	return tempInput;
}

//TODO: stop this from passing frame -1 in
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
			if (!it->set)
			{
				*it = input;
			}
			return;
		}
	}
}

void Input::SetLocalInput(FrameInput input)
{
	if (localInputs->back().frameNumber < input.frameNumber)
	{
		input.set = true;
		localInputs->erase(localInputs->begin());
		localInputs->push_back(input);
		return;
	}

	
	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (input.frameNumber == it->frameNumber)
		{
			if (!it->set)
			{
				*it = input;
			}
			return;
		}
	}
	
	InsertLocalInput(input);

}


void Input::InsertLocalInput(FrameInput input)
{
	int i = 0;
	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (input.frameNumber > it->frameNumber)
		{
			break;
		}
		i++;
	}

	input.set = true;
	auto it2 = localInputs->end() - i;
	localInputs->insert(it2, input);
}

void Input::PredictLocalInput(int frame)
{
	int i = 0;
	FrameInput predicted;
	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (frame == it->frameNumber)
		{
			if (it->set)
			{
				return;
			}
			else
			{
				it = it + 1;
				it->frameNumber = frame;
			}
		}
		else if (frame > it->frameNumber)
		{
			predicted = *it;
			break;
		}
		i++;
	}

	predicted.set = true;
	predicted.frameNumber = frame;
	auto it2 = localInputs->end() - i;
	localInputs->insert(it2, predicted);
}

void Input::ForceSet(int frame)
{
	for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (frame == it->frameNumber)
		{
			it->set = true;
			return;
		}
	}

	std::cout << "No input to force" << std::endl;

	/*for (std::vector<FrameInput>::reverse_iterator it = localInputs->rbegin(); it != localInputs->rend(); ++it)
	{
		if (frame == it->frameNumber)
		{
			it->set = true;
			return;
		}
	}*/
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
	if (GetOpponentInput(frame).set)
	{
		return true;
	}

	return false;
}


Input::~Input()
{
}
