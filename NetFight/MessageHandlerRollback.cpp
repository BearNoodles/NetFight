#include "MessageHandlerRollback.h"



MessageHandlerRollback::MessageHandlerRollback()
{
	
}


MessageHandlerRollback::~MessageHandlerRollback()
{
}


//bool MessageHandler::Initialise(sf::IpAddress ip, unsigned short oppPort, unsigned short ownPort)
bool MessageHandlerRollback::Initialise(sf::IpAddress ip, unsigned short oppPort, sf::UdpSocket* sock)
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

	FrameInput noInput;
	noInput.frameNumber = -1;
	for (bool b : noInput.inputs)
	{
		b = false;
	}

	//TODO WHEN INITIALISING SET ANOHTER BOOL TO SAY WHETHER THE INPUT HAS BEEN SET REMOTELY OR ONLY INITIALISED
	for (int i = 0; i < 6000; i++)
	{
		messages[i] = new Message();
		messages[i]->input1 = false;
		messages[i]->input2 = false;
		messages[i]->input3 = false;
		messages[i]->input4 = false;
		messages[i]->input5 = false;
		messages[i]->input6 = false;
		messages[i]->input7 = false;
		messages[i]->set = false;
		messages[i]->frame = i;
	}

	return true;
}

void MessageHandlerRollback::SetMinimumFrame(int minFrame)
{
	minimumFrame = minFrame;
}

void MessageHandlerRollback::SendNoInput(int frame)
{
	sf::Int32 frameSend;
	bool inputSend1 = false;
	bool inputSend2 = false;
	bool inputSend3 = false;
	bool inputSend4 = false;
	bool inputSend5 = false;
	bool inputSend6 = false;
	bool inputSend7 = false;

	frameSend = frame;

	sf::Packet packet;
	packet << inputSend1 << inputSend2 << inputSend3 << inputSend4 << inputSend5 << inputSend6 << inputSend7 << true << frameSend;


	//std::cout << "Send message failed" << std::endl;

	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}
}

void MessageHandlerRollback::SendFrameInput(FrameInput input)
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
	//bool set = true;

	/*for (int i = 0; i < 7; i++)
	{
	inputSend[i] = input.inputs[i];
	}*/



	frameSend = input.frameNumber;

	sf::Packet packet;
	packet << inputSend1 << inputSend2 << inputSend3 << inputSend4 << inputSend5 << inputSend6 << inputSend7 << true << frameSend;


	//std::cout << "Send message failed" << std::endl;

	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}

}

void MessageHandlerRollback::AddMessage(Message toAdd)
{
	Message* temp = &toAdd;

	if (messages[temp->frame]->set)
	{
		return;
	}

	/*if (messages.size() >= maxMessagesSize)
	{
	messages.erase(messages.begin());
	}*/
	messages[temp->frame]->input1 = temp->input1;
	messages[temp->frame]->input2 = temp->input2;
	messages[temp->frame]->input3 = temp->input3;
	messages[temp->frame]->input4 = temp->input4;
	messages[temp->frame]->input5 = temp->input5;
	messages[temp->frame]->input6 = temp->input6;
	messages[temp->frame]->input7 = temp->input7;

	messages[temp->frame]->set = true;


}

Message* MessageHandlerRollback::GetMessage(int frame)
{
	return messages[frame];
}

//FIX
//This should be called by Input to update the list of inputs from the opposing player
int MessageHandlerRollback::ReceiveInputMessages(int currentFrame)
{
	int rollbackFrame = -1;

	bool check = true;

	//Possibly time how long each new message takes to receive to calculate the delay required?
	//Set some kind of limit on how long the function should read messages
	int counter = 0;
	while (counter < 1000)
	{


		sf::Int32 frame;
		//bool inputs[7];
		bool input1, input2, input3, input4, input5, input6, input7, set;
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short port;
		if (socket->receive(packet, address, port) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			counter++;
			break;
		}

		//std::cout << "MESSAGE RECIEVED" << std::endl;

		if (address != opponentIP || port != opponentPort)
		{
			check = false;
		}

		//if ((packet >> inputs[7] >> frame) && check)
		if ((packet >> input1 >> input2 >> input3 >> input4 >> input5 >> input6 >> input7 >> set >> frame) && check)
		{

			/*if (frame < minimumFrame)
			{
				counter++;
				break;
			}*/

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
			msg.set = true;

			if (frame < currentFrame && messages[frame]->set == false)
			{
				if (frame < rollbackFrame || rollbackFrame == -1)
				{
					Message* temp = messages[frame];
					if (temp->input1 != msg.input1 || temp->input2 != msg.input2 || temp->input3 != msg.input3 || temp->input4 != msg.input4 || temp->input5 != msg.input5 || temp->input6 != msg.input6 || temp->input7 != msg.input7)
					{
						rollbackFrame = frame;
					}
				}
			}

			AddMessage(msg);

		}
		else if (check)
		{
			std::cout << "Couldnt receive message" << std::endl;
		}

		counter++;
	}

	
	if (!messages[currentFrame]->set && currentFrame > 0)
	{
		for (int i = currentFrame; i > 0; i--)
		{
			if (messages[i]->set)
			{
				for (int j = i + 1; j <= currentFrame; j++)
				{
					messages[j]->input1 = messages[i]->input1;
					messages[j]->input2 = messages[i]->input2;
					messages[j]->input3 = messages[i]->input3;
					messages[j]->input4 = messages[i]->input4;
					messages[j]->input5 = messages[i]->input5;
					messages[j]->input6 = messages[i]->input6;
					messages[j]->input7 = messages[i]->input7;
					messages[j]->set = false;
				}
				break;
			}
		}
	}

	return rollbackFrame;
}

//bool MessageHandlerRollback::CheckEarlyMessages()
//{
//	for (int i = 0; i < messagesEarly.size(); i++)
//	{
//		if (messagesEarly[i].frame == minimumFrame)
//		{
//			messages.push_back(messagesEarly[i]);
//			messagesEarly.erase(messagesEarly.begin() + i);
//			minimumFrame++;
//			return true;
//		}
//	}
//	return false;
//}

FrameInput MessageHandlerRollback::GetFrameInput(int frame)
{

	FrameInput newInput;
	/*for (int i = 0; i < 7; i++)
	{
		newInput.inputs[i] = false;
	}
	newInput.frameNumber = -1;*/

	//TODO: FIX RECIEVING UNINITIALIZED MESSAGES

	newInput.inputs[0] = messages[frame]->input1;
	newInput.inputs[1] = messages[frame]->input2;
	newInput.inputs[2] = messages[frame]->input3;
	newInput.inputs[3] = messages[frame]->input4;
	newInput.inputs[4] = messages[frame]->input5;
	newInput.inputs[5] = messages[frame]->input6;
	newInput.inputs[6] = messages[frame]->input7;

	newInput.frameNumber = messages[frame]->frame;

	/*for (std::vector<Message>::reverse_iterator it = messages.rbegin(); it != messages.rend(); ++it)
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
	}*/

	return newInput;
}