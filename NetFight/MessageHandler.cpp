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
}

void MessageHandler::SendFrameInput(FrameInput input)
{

}

FrameInput MessageHandler::ReceiveFrameInput()
{
	FrameInput t;
	t.frameNumber = 0;
	for (int i = 0; i < 7; i++)
	{
		t.inputs[i] = false;
	}
	return t;
}