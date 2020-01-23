#pragma once

#include "Input.h"
#include "Message.h"
#include <SFML/Network.hpp>
#include <list>
#include <iostream>

class MessageHandler
{
public:

	MessageHandler();
	~MessageHandler();

	void Initialise(sf::IpAddress ip, unsigned short port);

	void SendFrameInput(FrameInput input);
	FrameInput ReceiveFrameInput(int frame);

	void SetMinimumFrame(int minFrame);

private:

	//sf::IpAddress opponentIP;

	sf::UdpSocket socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort;

	std::list<Message> messages;
	int maxMessagesSize;

	void AddMessage(Message toAdd);

	int minimumFrame;
};

