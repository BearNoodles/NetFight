#pragma once

#include "Input.h"
#include "Message.h"
#include <SFML/Network.hpp>
#include <list>
#include <vector>
#include <iostream>

#define MAXMESSAGE 6500

class MessageHandler
{
public:

	MessageHandler();
	~MessageHandler();

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
	void ResetPing();

	void Reset();

	void SendRestartMessage();

	bool GetRestartReceived();

	void ExhaustAllMessages();

private:


	bool restartRecieved = false;

	sf::UdpSocket* socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort;

	Message* messages[MAXMESSAGE];
	std::vector<Message> messagesEarly;
	int maxMessagesSize;

	void AddMessage(Message toAdd);
	Message* GetMessage(int frame);

	bool CheckEarlyMessages();

	int minimumFrame;

	int lastSent;
	int lastReceived;

	int pingCheckFrame;
	bool pingChecked;
	bool pingReceived;
};
