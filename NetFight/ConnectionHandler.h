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
	bool WaitForPlayers();

private:
	bool InitHost();
	bool InitClient();

	int ID;

	sf::UdpSocket socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort;

};

