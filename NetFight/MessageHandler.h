#pragma once

#include "Input.h"
#include "Message.h"
#include <SFML/Network.hpp>
class MessageHandler
{
public:

	MessageHandler();
	~MessageHandler();

	void Initialise(sf::IpAddress ip, unsigned short port);

	void SendFrameInput(FrameInput input);
	FrameInput ReceiveFrameInput();

private:

	//sf::IpAddress opponentIP;

	sf::UdpSocket socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort;
};

