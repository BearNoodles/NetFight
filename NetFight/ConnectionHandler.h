#pragma once
#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include "Input.h"

class ConnectionHandler
{
public:
	ConnectionHandler();
	~ConnectionHandler();

	int HostOrClient();
	bool IsRollBackOn();
	bool WaitForPlayers();

	sf::IpAddress GetOpponentIP();
	unsigned short GetOpponentPort();
	unsigned short GetOwnPort();

	sf::UdpSocket* GetSocket();

	int GetLocalPlayerNumber();
private:
	bool InitHost();
	bool InitClient();

	int ID;

	bool rollback;

	sf::UdpSocket* socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort = 54444;
};

