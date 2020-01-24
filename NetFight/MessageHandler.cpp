#include "MessageHandler.h"

MessageHandler::MessageHandler()
{

}

MessageHandler::~MessageHandler()
{

}

void MessageHandler::Initialise(sf::IpAddress ip, unsigned short port)
{
	opponentIP = ip;
	opponentPort = port;

	minimumFrame = 0;

	maxMessagesSize = 1000;
}

void MessageHandler::SetMinimumFrame(int minFrame)
{
	minimumFrame = minFrame;
}

void MessageHandler::SendFrameInput(FrameInput input)
{

	sf::Int8 frameSend;
	bool inputSend[7];

	for (int i = 0; i < 7; i++)
	{
		inputSend[i] = input.inputs[i];
	}

	frameSend = input.frameNumber;

	sf::Packet packet;
	packet << inputSend << frameSend;

	if (socket.send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}

}

void MessageHandler::AddMessage(Message toAdd)
{
	if (messages.size() >= maxMessagesSize)
	{
		messages.erase(messages.begin());
	}
	messages.push_back(toAdd);
}

//FIX
//This should be called by Input to update the list of inputs from the opposing player
void MessageHandler::ReceiveInputMessages(int frame)
{	
	bool check = true;
	
	//Possibly time how long each new message takes to receive to calculate the delay required?
	//Set some kind of limit on how long the function should read messages
	while (true)
	{
		sf::Int8 frame;
		bool inputs[7];
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short port;
		if (socket.receive(packet, address, port) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			check = false;
			break;
		}
		if (address != opponentIP || port != opponentPort)
		{
			check = false;
		}
		if ((packet >> inputs[7] >> frame) && check)
		{

			if (frame < minimumFrame)
			{
				continue;
			}
			
			//good
			Message msg;
			for (int i = 0; i < 7; i++)
			{
				msg.inputs[i] = inputs[i];
			}

			msg.frame = frame;

			if (frame == minimumFrame)
			{
				messages.push_back(msg);
				minimumFrame++;
				while (true)
				{
					if (!CheckEarlyMessages())
					{
						break;
					}
				}
			}
			else if(frame > minimumFrame)
			{
				messagesEarly.push_back(msg);
			}


		}
		else if (check)
		{
			std::cout << "Couldnt receive message" << std::endl;
		}
	}
}

bool MessageHandler::CheckEarlyMessages()
{
	for (int i = 0; i < messagesEarly.size(); i++)
	{
		if (messagesEarly[i].frame == minimumFrame)
		{
			messages.push_back(messagesEarly[i]);
			messagesEarly.erase(messagesEarly.begin() + i);
			minimumFrame++;
			return true;
		}
	}
	return false;
}

FrameInput MessageHandler::GetFrameInput(int frame)
{

	FrameInput newInput;
	for (int i = 0; i < 7; i++)
	{
		newInput.inputs[i] = false;
	}
	newInput.frameNumber = -1;
	for (Message m : messages)
	{
		if (m.frame == frame)
		{
			FrameInput newInput;
			for (int i = 0; i < 7; i++)
			{
				newInput.inputs[i] = m.inputs[i];
			}
			newInput.frameNumber = m.frame;
		}
	}

	return newInput;
}