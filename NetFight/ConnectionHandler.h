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


	bool SetupHost();
	bool SetupClient();

	void SetRollback(bool value);
	bool GetRollback();

	bool StartGame();

	void SetID(int val);
private:
	bool InitHost();
	bool InitClient();

	int ID;

	bool rollback;

	sf::UdpSocket* socket;
	sf::IpAddress opponentIP;
	unsigned short opponentPort = 54444;
};

