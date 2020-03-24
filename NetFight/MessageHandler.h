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

	//bool Initialise(sf::IpAddress ip, unsigned short opponentPort, unsigned short ownPort);
	bool Initialise(sf::IpAddress ip, unsigned short opponentPort, sf::UdpSocket* sock);

	void SendFrameInput(FrameInput input);
	void ReceiveMessagesDelay();
	int ReceiveMessagesRollback(int currentFrame);

	void SendPingReply(int frame);

	void SendNoInput(int frame);

	void SetMinimumFrame(int minFrame);

	FrameInput GetFrameInput(int frame);

	int CalculateDelay();

	bool CheckPing();

	void Reset();

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

	void UpdateMinFrame();
	int minimumFrame;

	int lastSent;
	int lastReceived;

	int pingCheckFrame;
	bool pingChecked;
	bool pingReceived;
};
