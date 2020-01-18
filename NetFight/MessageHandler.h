#pragma once

#include "Input.h"
#include <SFML/Network.hpp>
class MessageHandler
{
public:

	MessageHandler();
	~MessageHandler();

	void SendFrameInput(FrameInput input);
	FrameInput ReceiveFrameInput();

private:

	//sf::IpAddress opponentIP;

	sf::UdpSocket socket;
	sf::IpAddress hostIP = "127.0.0.1";
	unsigned short hostPort = 54444;

	sf::IpAddress clientIP;
	unsigned short clientPort;
};

