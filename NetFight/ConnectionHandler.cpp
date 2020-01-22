#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler()
{
	socket.setBlocking(false);
}


ConnectionHandler::~ConnectionHandler()
{
}

int ConnectionHandler::HostOrClient()
{

	while (true)
	{
		std::string choice;
		std::cout << "Type 1 to start a new game or enter IP of host (press 2 for default host)" << std::endl;
		std::cin >> choice;

		if (choice == "1")
		{
			ID = 1;

			if (!InitHost())
			{
				std::cout << "Error starting as host, try again" << std::endl;
				continue;
			}

			return 1;
			//break;
		}
		else if (choice == "2")
		{
			ID = 2;


			if (!InitClient())
			{
				std::cout << "Error connecting to host, try again" << std::endl;
				continue;
			}
			return 2;
			//break;
		}
		else
		{
			opponentIP = choice;

			std::cout << "Enter port number of host" << std::endl;
			unsigned short portChoice;
			std::cin >> portChoice;
			opponentPort = portChoice;
			if (!InitClient())
			{
				std::cout << "Error connecting to host, try again" << std::endl;
				continue;
			}
			return 2;
			//break;
		}

	}
}

bool ConnectionHandler::InitHost()
{

	// bind the socket to a port
	if (socket.bind(opponentPort) != sf::Socket::Done)
	{
		return false;
	}


	ID = 1;
	return true;
}

bool ConnectionHandler::InitClient()
{
	int counter = 0;
	// bind the socket to a port
	if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		// error...
	}

	while (true)
	{
		if (counter > 10000)
		{
			std::cout << "Could not connect to host" << std::endl;
			return false;
		}
		std::string s = "hello";

		sf::Packet packet;
		packet << s;

		if (socket.send(packet, opponentIP, opponentPort) != sf::Socket::Done)
		{
			// error...
			//send failed try it again
			std::cout << "Send greeting failed, retrying" << std::endl;
			counter++;
			continue;
		}
		counter = 0;
		packet.clear();
		break;
	}
	while (true)
	{
		if (counter > 10000)
		{
			std::cout << "Could not connect to host" << std::endl;
			return false;
		}
		std::string r;
		sf::Packet packet;
		if (socket.receive(packet, opponentIP, opponentPort) != sf::Socket::Done)
		{
			// error...
			//receive failed send hello again
			std::cout << "Receive failed, retrying" << std::endl;
			counter++;
			continue;
		}

		packet >> r;

		if (std::stoi(r) > 1)
		{
			ID = std::stoi(r);
			break;
		}
		else
		{
			std::cout << "Didnt receive number > 1, retrying" << std::endl;
		}
	}

	return true;
}



bool ConnectionHandler::WaitForPlayers()
{
	//TODO draw other players while waiting
	if (ID == 1)
	{
		//start game if host presses space
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			std::string s = "begin";
			sf::Packet packet;
			packet << s;

			if (socket.send(packet, opponentIP, opponentPort) != sf::Socket::Done)
			{
				// error...
				//send failed try it again
				std::cout << "Send begin failed, try again" << std::endl;
				return false;
			}
			return true;
		}
		sf::IpAddress senderIP;
		unsigned short senderPort;
		sf::Packet packet;
		std::string greeting;

		if (socket.receive(packet, senderIP, senderPort) != sf::Socket::Done)
		{
			// error...
			//std::cout << "no messages yet" << std::endl;
			return false;
		}

		if (packet >> greeting)
		{
			//good
		}
		else
		{
			std::cout << "couldnt read packet" << std::endl;
			return false;
		}

		if (greeting == "hello")
		{

			opponentIP = senderIP;
			opponentPort = senderPort;
			packet.clear();

			//std::string s = std::to_string(playerCount + 1);
			std::string s = std::to_string(1 + 1);
			packet << s;
			int counter = 0;
			while (true)
			{
				if (counter > 10000)
				{
					std::cout << "Error, could not connect to client" << std::endl;
					break;
				}
				if (socket.send(packet, opponentIP, opponentPort) != sf::Socket::Done)
				{
					// error...
					//recieve failed send hello again
					std::cout << "playerCount send failed, rety" << std::endl;
					counter++;
					continue;
				}
				std::cout << "Player has joined, press space to begin" << std::endl;

				break;
			}

		}
	}

	else
	{
		sf::IpAddress hostIP;
		unsigned short hostPort;
		sf::Packet packet;
		std::string beginMessage;
		if (socket.receive(packet, hostIP, hostPort) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			return false;
		}

		packet >> beginMessage;
		if (beginMessage == "begin")
		{
			return true;
		}
	}
	return false;
}