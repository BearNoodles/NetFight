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

	sf::IpAddress GetOpponentIP();
	unsigned short GetOpponentPort();
	unsigned short GetOwnPort();

private:
	bool InitHost();
	bool InitClient();

	int ID;

	sf::UdpSocket socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort = 54444;
	//TODO: get client port from sf::anyport
	unsigned short ownPort = 54444;

};

