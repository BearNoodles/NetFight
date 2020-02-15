#pragma once
#include "Input.h"
#include "Message.h"
#include <SFML/Network.hpp>
#include <list>
#include <vector>
#include <iostream>

class MessageHandlerRollback
{
public:

	MessageHandlerRollback();
	~MessageHandlerRollback();

	//bool Initialise(sf::IpAddress ip, unsigned short opponentPort, unsigned short ownPort);
	bool Initialise(sf::IpAddress ip, unsigned short opponentPort, sf::UdpSocket* sock);

	void SendFrameInput(FrameInput input);
	int ReceiveInputMessages(int currentFrame);

	void SendNoInput(int frame);

	void SetMinimumFrame(int minFrame);

	FrameInput GetFrameInput(int frame);

private:

	//sf::IpAddress opponentIP;

	sf::UdpSocket* socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort;

	//std::vector<Message> messages;
	Message* messages[6000];
	std::vector<Message> messagesEarly;
	int maxMessagesSize;

	void AddMessage(Message toAdd);
	Message* GetMessage(int frame);

	bool CheckEarlyMessages();
	int minimumFrame;
};
