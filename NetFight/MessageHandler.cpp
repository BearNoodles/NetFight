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

}

void MessageHandler::AddMessage(Message toAdd)
{
	if (messages.size() >= maxMessagesSize)
	{
		messages.pop_front();
	}
	messages.push_back(toAdd);
}

//FIX
//This should be called by Input to update the list of inputs from the opposing player
FrameInput MessageHandler::ReceiveFrameInput(int frame)
{
	Message m;
	sf::Packet packet;
	if (socket.receive(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//receive failed send hello again
		/*std::cout << "Receive failed, retrying" << std::endl;
		counter++;
		continue;*/
	}
	
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
			check == false;
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

			messages.push_back(msg);

		}
		else if (check)
		{
			std::cout << "Couldnt receive message" << std::endl;
		}
	}
	
	

	//give messages to appropriate players
	//while (!messages.empty())
	//{
	//	if (messages.front().id == opponent.GetID())
	//	{
	//		opponent.AddMessage(messages.front());
	//		messages.pop_front();
	//	}
	//	else if (messages.front().id == ballID && myID != 1)
	//	{
	//		ball.AddMessage(messages.front());
	//		messages.pop_front();
	//	}
	//	//id 100 means a goal was scored
	//	else if (messages.front().id == 100 && myID != 1)
	//	{
	//		while (true)
	//		{
	//			Message msg;
	//			sf::Int8 id = 100;
	//			float x = 0;
	//			float y = 0;
	//			float dx = 0;
	//			float dy = 0;
	//			float timeSent = 0;
	//			sf::Packet packet;
	//			packet << id << x << y << dx << dy << timeSent;

	//			if (socket.send(packet, hostIP, hostPort) != sf::Socket::Done)
	//			{
	//				// error...
	//				//send failed try it again
	//				std::cout << "Send message failed" << std::endl;
	//				continue;
	//			}
	//			break;
	//		}
	//		player1Score += messages.front().x;
	//		player2Score += messages.front().y;
	//		Reset();
	//		messages.pop_front();
	//	}
	//	//id 99 means client hit ball
	//	else if (messages.front().id == 99 && myID != 1)
	//	{
	//		sf::Vector2f ballVelocity = ball.GetVelocity();
	//		myPlayer.SetVelocity(sf::Vector2f((ball.GetVelocity().x) / 2, (ball.GetVelocity().y / 2)));
	//		float totalSpeed = MagnitudeVector2(myPlayer.GetVelocity()) + MagnitudeVector2(ballVelocity);
	//		sf::Vector2f pdir = NormaliseVector2(myPlayer.GetSprite().getPosition() - ball.GetSprite().getPosition());
	//		sf::Vector2f newVel = pdir * (totalSpeed / 4);

	//		myPlayer.SetVelocity(sf::Vector2f((myPlayer.GetVelocity().x + newVel.x) / 2, (myPlayer.GetVelocity().y + newVel.y) / 2));

	//		ball.SetVelocity(sf::Vector2f(messages.front().x, messages.front().y));
	//		messages.pop_front();
	//	}
	//	else
	//	{
	//		messages.pop_front();
	//	}
	//}

}