#pragma once

#include "Input.h"
#include "Message.h"
#include <SFML/Network.hpp>
#include <list>
#include <vector>
#include <iostream>

class MessageHandler
{
public:

	MessageHandler();
	~MessageHandler();

	void Initialise(sf::IpAddress ip, unsigned short port);

	void SendFrameInput(FrameInput input);
	void ReceiveInputMessages(int frame);

	void SetMinimumFrame(int minFrame);

	FrameInput GetFrameInput(int frame);

private:

	//sf::IpAddress opponentIP;

	sf::UdpSocket socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort;

	std::vector<Message> messages;
	std::vector<Message> messagesEarly;
	int maxMessagesSize;

	void AddMessage(Message toAdd);

	bool CheckEarlyMessages();
	int minimumFrame;
};

