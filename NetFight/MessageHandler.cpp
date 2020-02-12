#include "MessageHandler.h"

MessageHandler::MessageHandler()
{

}

MessageHandler::~MessageHandler()
{

}

//bool MessageHandler::Initialise(sf::IpAddress ip, unsigned short oppPort, unsigned short ownPort)
bool MessageHandler::Initialise(sf::IpAddress ip, unsigned short oppPort, sf::UdpSocket* sock)
{
	socket = sock;
	//socket.setBlocking(false);
	opponentIP = ip;
	opponentPort = oppPort;

	

	//TODO: get client port from connectionhandler
	/*if (socket.bind(ownPort) != sf::Socket::Done)
	{
		std::cout << "Message Handler socket not bound" << std::endl;
		return false;
	}*/

	maxMessagesSize = 1000;

	return true;
}

void MessageHandler::SetMinimumFrame(int minFrame)
{
	minimumFrame = minFrame;
}

void MessageHandler::SendFrameInput(FrameInput input)
{

	sf::Int32 frameSend;
	//bool inputSend[7];
	bool inputSend1 = input.inputs[0];
	bool inputSend2 = input.inputs[1];
	bool inputSend3 = input.inputs[2];
	bool inputSend4 = input.inputs[3];
	bool inputSend5 = input.inputs[4];
	bool inputSend6 = input.inputs[5];
	bool inputSend7 = input.inputs[6];

	/*for (int i = 0; i < 7; i++)
	{
		inputSend[i] = input.inputs[i];
	}*/



	frameSend = input.frameNumber;

	sf::Packet packet;
	packet << inputSend1 << inputSend2 << inputSend3 << inputSend4 << inputSend5 << inputSend6 << inputSend7 << frameSend;


	//std::cout << "Send message failed" << std::endl;

	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}

}

void MessageHandler::AddMessage(Message toAdd)
{
	Message* temp = &toAdd;
	if (messages.size() >= maxMessagesSize)
	{
		messages.erase(messages.begin());
	}
	messages.push_back(*temp);
}

//FIX
//This should be called by Input to update the list of inputs from the opposing player
void MessageHandler::ReceiveInputMessages(int frame)
{	
	bool check = true;
	
	//Possibly time how long each new message takes to receive to calculate the delay required?
	//Set some kind of limit on how long the function should read messages
	int counter = 0;
	while (counter < 10)
	{


		sf::Int32 frame;
		//bool inputs[7];
		bool input1, input2, input3, input4, input5, input6, input7;
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short port;
		if (socket->receive(packet, address, port) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			counter++;
			continue;
		}

		std::cout << "MESSAGE RECIEVED" << std::endl;

		if (address != opponentIP || port != opponentPort)
		{
			check = false;
		}

		//if ((packet >> inputs[7] >> frame) && check)
		if ((packet >> input1 >> input2 >> input3 >> input4 >> input5 >> input6 >> input7 >> frame) && check)
		{

			if (frame < minimumFrame)
			{
				counter++;
				break;
			}
			
			//good
			Message msg;
			msg.input1 = input1;
			msg.input2 = input2;
			msg.input3 = input3;
			msg.input4 = input4;
			msg.input5 = input5;
			msg.input6 = input6;
			msg.input7 = input7;
			/*for (int i = 0; i < 7; i++)
			{
				msg.inputs[i] = inputs[i];
			}*/

			msg.frame = frame;

			if (frame == minimumFrame)
			{
				AddMessage(msg);
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

		counter++;
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


	for (std::vector<Message>::reverse_iterator it = messages.rbegin(); it != messages.rend(); ++it)
	{
		if (it->frame == frame)
		{
			newInput.inputs[0] = it->input1;
			newInput.inputs[1] = it->input2;
			newInput.inputs[2] = it->input3;
			newInput.inputs[3] = it->input4;
			newInput.inputs[4] = it->input5;
			newInput.inputs[5] = it->input6;
			newInput.inputs[6] = it->input7;

			newInput.frameNumber = it->frame;

			break;
		}
	}

	//for (Message m : messages)
	//{
	//	if (m.frame == frame)
	//	{
	//		/*for (int i = 0; i < 7; i++)
	//		{
	//			newInput.inputs[i] = m.inputs[i];
	//		}*/
	//		newInput.inputs[0] = m.input1;
	//		newInput.inputs[1] = m.input2;
	//		newInput.inputs[2] = m.input3;
	//		newInput.inputs[3] = m.input4;
	//		newInput.inputs[4] = m.input5;
	//		newInput.inputs[5] = m.input6;
	//		newInput.inputs[6] = m.input7;

	//		newInput.frameNumber = m.frame;

	//		break;
	//	}
	//}

	return newInput;
}