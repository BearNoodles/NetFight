#include "MessageHandler.h"



MessageHandler::MessageHandler()
{

}


MessageHandler::~MessageHandler()
{
}

void MessageHandler::Reset()
{
	//TODO WHEN INITIALISING SET ANOHTER BOOL TO SAY WHETHER THE INPUT HAS BEEN SET REMOTELY OR ONLY INITIALISED
	for (int i = 0; i < MAXMESSAGE; i++)
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

	lastSent = 0;
	lastReceived = 0;

	pingCheckFrame = 0;
	pingChecked = true;
	pingReceived = false;

	minimumFrame = 0;

	restartRecieved = false;

	while (true)
	{
		sf::Int32 frame;
		//bool inputs[7];
		int pingFrame;
		bool pingSend, pingReply, input1, input2, input3, input4, input5, input6, input7, set;
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short port;
		if (socket->receive(packet, address, port) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			//counter++;
			break;
		}
	}
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

	FrameInput noInput;
	noInput.frameNumber = -1;
	for (bool b : noInput.inputs)
	{
		b = false;
	}

	//TODO WHEN INITIALISING SET ANOHTER BOOL TO SAY WHETHER THE INPUT HAS BEEN SET REMOTELY OR ONLY INITIALISED
	for (int i = 0; i < MAXMESSAGE; i++)
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

	lastSent = 0;
	lastReceived = 0;

	pingCheckFrame = 0;
	pingChecked = true;

	return true;
}

void MessageHandler::SetMinimumFrame(int minFrame)
{
	minimumFrame = minFrame;
}

void MessageHandler::SendNoInput(int frame)
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

	if (frameSend > lastSent)
	{
		lastSent = frameSend;
	}
}

void MessageHandler::SendFrameInput(FrameInput input)
{

	sf::Int32 frameSend;
	//bool inputSend[7];

	bool pingSend = false;
	bool pingReply = false;

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

	if (pingChecked && frameSend > pingCheckFrame)
	{
		pingCheckFrame = frameSend;
		pingChecked = false;
		pingSend = true;
	}


	sf::Packet packet;
	packet << pingSend << pingReply << inputSend1 << inputSend2 << inputSend3 << inputSend4 << inputSend5 << inputSend6 << inputSend7 << true << frameSend;


	//std::cout << "Send message failed" << std::endl;

	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}
}


void MessageHandler::SendPingReply(int frame)
{

	sf::Int32 frameSend;

	bool pingReply = true;


	frameSend = frame;

	sf::Packet packet;
	packet << false << pingReply << false << false << false << false << false << false << false << false << frame;


	//std::cout << "Send message failed" << std::endl;

	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}
}

void MessageHandler::SendRestartMessage()
{
	sf::Int32 frameSend;

	frameSend = -10;

	sf::Packet packet;
	packet << false << false << false << false << false << false << false << false << false << false << frameSend;


	//std::cout << "Send message failed" << std::endl;

	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
	{
		// error...
		//send failed try it again
		std::cout << "Send message failed" << std::endl;
	}
}

bool MessageHandler::GetRestartReceived()
{
	return restartRecieved;
}

void MessageHandler::AddMessage(Message toAdd)
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

Message* MessageHandler::GetMessage(int frame)
{
	return messages[frame];
}

void MessageHandler::ExhaustAllMessages()
{
	
}

void MessageHandler::ReceiveMessagesDelay()
{
	bool check = true;

	//Possibly time how long each new message takes to receive to calculate the delay required?
	//Set some kind of limit on how long the function should read messages
	int counter = 0;
	while (counter < 100)
	{
		//std::cout << "Count: " << counter << std::endl;
		
		sf::Int32 frame;
		//bool inputs[7];
		int pingFrame;
		bool pingSend, pingReply, input1, input2, input3, input4, input5, input6, input7, set;
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short port;
		if (socket->receive(packet, address, port) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			//counter++;
			break;
		}

		//std::cout << "MESSAGE RECIEVED" << std::endl;

		if (address != opponentIP || port != opponentPort)
		{
			check = false;
		}

		//if ((packet >> inputs[7] >> frame) && check)
		if ((packet >> pingSend >> pingReply >> input1 >> input2 >> input3 >> input4 >> input5 >> input6 >> input7 >> set >> frame) && check)
		{
			if (frame == -10)
			{
				restartRecieved = true;
				return;
			}

			if (pingSend)
			{
				SendPingReply(frame);
			}

			else if (pingReply)
			{
				if (frame == pingCheckFrame)
				{
					pingReceived = true;
					counter++;
					continue;
				}
			}

			if (frame < minimumFrame)
			{
				counter++;
				continue;
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
			msg.set = true;


			AddMessage(msg);


			if (frame == minimumFrame)
			{
				minimumFrame++;
			}
		}

		else if (check)
		{
			std::cout << "Couldnt receive message" << std::endl;
		}

		counter++;
	}
}

//FIX
//This should be called by Input to update the list of inputs from the opposing player
int MessageHandler::ReceiveMessagesRollback(int currentFrame)
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
		int pingFrame;
		bool pingSend, pingReply, input1, input2, input3, input4, input5, input6, input7, set;
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short port;
		if (socket->receive(packet, address, port) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			//counter++;
			break;
		}

		//std::cout << "MESSAGE RECIEVED" << std::endl;

		if (address != opponentIP || port != opponentPort)
		{
			check = false;
		}

		//if ((packet >> inputs[7] >> frame) && check)
		if ((packet >> pingSend >> pingReply >> input1 >> input2 >> input3 >> input4 >> input5 >> input6 >> input7 >> set >> frame) && check)
		{
			if (frame == -10)
			{
				restartRecieved = true;
				continue;
			}

			if (pingSend)
			{
				SendPingReply(frame);
			}

			else if (pingReply)
			{
				if (frame == pingCheckFrame)
				{
					pingReceived = true;
					counter++;
					continue;
				}
			}
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

FrameInput MessageHandler::GetFrameInput(int frame)
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

	newInput.set = messages[frame]->set;

	return newInput;
}

int MessageHandler::CalculateDelay()
{
	return lastSent - lastReceived;
}

bool MessageHandler::CheckPing()
{
	if (pingReceived)
	{
		pingChecked = true;
		pingReceived = false;
		return true;
	}
	return pingReceived;
}


//#include "MessageHandler.h"
//
//MessageHandler::MessageHandler()
//{
//	
//}
//
//MessageHandler::~MessageHandler()
//{
//
//}
//
////bool MessageHandler::Initialise(sf::IpAddress ip, unsigned short oppPort, unsigned short ownPort)
//bool MessageHandler::Initialise(sf::IpAddress ip, unsigned short oppPort, sf::UdpSocket* sock)
//{
//	socket = sock;
//	//socket.setBlocking(false);
//	opponentIP = ip;
//	opponentPort = oppPort;
//
//	
//
//	//TODO: get client port from connectionhandler
//	/*if (socket.bind(ownPort) != sf::Socket::Done)
//	{
//		std::cout << "Message Handler socket not bound" << std::endl;
//		return false;
//	}*/
//
//	maxMessagesSize = 1000;
//
//	FrameInput noInput;
//	noInput.frameNumber = -1;
//	for (bool b : noInput.inputs)
//	{
//		b = false;
//	}
//	for (int i = 0; i < 6000; i++)
//	{
//		messages[i] = new Message();
//		messages[i]->input1 = false;
//		messages[i]->input2 = false;
//		messages[i]->input3 = false;
//		messages[i]->input4 = false;
//		messages[i]->input5 = false;
//		messages[i]->input6 = false;
//		messages[i]->input7 = false;
//		messages[i]->set = false;
//		messages[i]->frame = i;
//	}
//
//	return true;
//}
//
//void MessageHandler::SetMinimumFrame(int minFrame)
//{
//	minimumFrame = minFrame;
//}
//
//void MessageHandler::SendNoInput(int frame)
//{
//	sf::Int32 frameSend;
//	bool inputSend1 = false;
//	bool inputSend2 = false;
//	bool inputSend3 = false;
//	bool inputSend4 = false;
//	bool inputSend5 = false;
//	bool inputSend6 = false;
//	bool inputSend7 = false;
//
//	frameSend = frame;
//
//	sf::Packet packet;
//	packet << inputSend1 << inputSend2 << inputSend3 << inputSend4 << inputSend5 << inputSend6 << inputSend7 << frameSend;
//
//
//	//std::cout << "Send message failed" << std::endl;
//
//	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
//	{
//		// error...
//		//send failed try it again
//		std::cout << "Send message failed" << std::endl;
//	}
//}
//
//void MessageHandler::SendFrameInput(FrameInput input)
//{
//	sf::Int32 frameSend;
//
//	bool inputSend1 = input.inputs[0];
//	bool inputSend2 = input.inputs[1];
//	bool inputSend3 = input.inputs[2];
//	bool inputSend4 = input.inputs[3];
//	bool inputSend5 = input.inputs[4];
//	bool inputSend6 = input.inputs[5];
//	bool inputSend7 = input.inputs[6];
//
//	/*for (int i = 0; i < 7; i++)
//	{
//		inputSend[i] = input.inputs[i];
//	}*/
//
//
//
//	frameSend = input.frameNumber;
//
//	sf::Packet packet;
//	packet << inputSend1 << inputSend2 << inputSend3 << inputSend4 << inputSend5 << inputSend6 << inputSend7 << frameSend;
//
//
//	//std::cout << "Send message failed" << std::endl;
//
//	if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
//	{
//		// error...
//		//send failed try it again
//		std::cout << "Send message failed" << std::endl;
//	}
//
//}
//
//void MessageHandler::AddMessage(Message toAdd)
//{
//	Message* temp = &toAdd;
//
//	if (messages[temp->frame]->set)
//	{
//		return;
//	}
//
//	/*if (messages.size() >= maxMessagesSize)
//	{
//	messages.erase(messages.begin());
//	}*/
//	messages[temp->frame]->input1 = temp->input1;
//	messages[temp->frame]->input2 = temp->input2;
//	messages[temp->frame]->input3 = temp->input3;
//	messages[temp->frame]->input4 = temp->input4;
//	messages[temp->frame]->input5 = temp->input5;
//	messages[temp->frame]->input6 = temp->input6;
//	messages[temp->frame]->input7 = temp->input7;
//
//	messages[temp->frame]->set = true;
//}
//
//Message* MessageHandler::GetMessage(int frame)
//{
//	return messages[frame];
//}
//
////FIX
////This should be called by Input to update the list of inputs from the opposing player
//
////TODO: WRITE A NEW RECEIVE FUNCTION FOR ROLLBACK
//void MessageHandler::ReceiveInputMessages()
//{	
//	bool check = true;
//	
//	//Possibly time how long each new message takes to receive to calculate the delay required?
//	//Set some kind of limit on how long the function should read messages
//	int counter = 0;
//	while (counter < 1000)
//	{
//
//
//		sf::Int32 frame;
//		//bool inputs[7];
//		bool input1, input2, input3, input4, input5, input6, input7;
//		sf::Packet packet;
//		sf::IpAddress address;
//		unsigned short port;
//		if (socket->receive(packet, address, port) != sf::Socket::Done)
//		{
//			// error...
//			//recieve failed send hello again
//			//std::cout << "no messages yet" << std::endl;
//			//counter++;
//			break;
//		}
//
//		std::cout << "MESSAGE RECIEVED" << std::endl;
//
//		if (address != opponentIP || port != opponentPort)
//		{
//			check = false;
//		}
//
//		//if ((packet >> inputs[7] >> frame) && check)
//		if ((packet >> input1 >> input2 >> input3 >> input4 >> input5 >> input6 >> input7 >> frame) && check)
//		{
//
//			if (frame < minimumFrame)
//			{
//				counter++;
//				break;
//			}
//			
//			//good
//			Message msg;
//			msg.input1 = input1;
//			msg.input2 = input2;
//			msg.input3 = input3;
//			msg.input4 = input4;
//			msg.input5 = input5;
//			msg.input6 = input6;
//			msg.input7 = input7;
//			/*for (int i = 0; i < 7; i++)
//			{
//				msg.inputs[i] = inputs[i];
//			}*/
//
//			msg.frame = frame;
//			msg.set = true;
//
//
//			AddMessage(msg);
//
//
//			if (frame == minimumFrame)
//			{
//				minimumFrame++;
//			}
//		}
//		else if (check)
//		{
//			std::cout << "Couldnt receive message" << std::endl;
//		}
//
//		counter++;
//	}
//
//}
//
//
////bool MessageHandler::CheckEarlyMessages()
////{
////	for (int i = 0; i < messagesEarly.size(); i++)
////	{
////		if (messagesEarly[i].frame == minimumFrame)
////		{
////			messages.push_back(messagesEarly[i]);
////			messagesEarly.erase(messagesEarly.begin() + i);
////			minimumFrame++;
////			return true;
////		}
////	}
////	return false;
////}
//
//FrameInput MessageHandler::GetFrameInput(int frame)
//{
//
//	FrameInput newInput;
//
//	newInput.inputs[0] = messages[frame]->input1;
//	newInput.inputs[1] = messages[frame]->input2;
//	newInput.inputs[2] = messages[frame]->input3;
//	newInput.inputs[3] = messages[frame]->input4;
//	newInput.inputs[4] = messages[frame]->input5;
//	newInput.inputs[5] = messages[frame]->input6;
//	newInput.inputs[6] = messages[frame]->input7;
//
//	newInput.frameNumber = messages[frame]->frame;
//
//	/*for (std::vector<Message>::reverse_iterator it = messages.rbegin(); it != messages.rend(); ++it)
//	{
//		if (it->frame == frame)
//		{
//			newInput.inputs[0] = it->input1;
//			newInput.inputs[1] = it->input2;
//			newInput.inputs[2] = it->input3;
//			newInput.inputs[3] = it->input4;
//			newInput.inputs[4] = it->input5;
//			newInput.inputs[5] = it->input6;
//			newInput.inputs[6] = it->input7;
//
//			newInput.frameNumber = it->frame;
//
//			break;
//		}
//	}*/
//
//	return newInput;
//}