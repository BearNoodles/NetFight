#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler()
{
	ID = 1;

	//Set up socket
	socket = new sf::UdpSocket();
	socket->setBlocking(false);

	rollback = false;
}


ConnectionHandler::~ConnectionHandler()
{
}

//Sets the local user to be a host or client, for connecting purposes
int ConnectionHandler::HostOrClient()
{

	while (true)
	{
		//Choose whether to host or search for a host
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

			//Choose which type of netcode to use
			while (true)
			{
				std::cout << "Type 1 to use rollback or press 2 to use delay" << std::endl;
				std::cin >> choice;
				if (choice == "1")
				{
					rollback = true;
					break;
				}
				else if (choice == "2")
				{
					rollback = false;
					break;
				}
				else
				{
					std::cout << "Enter a valid choice" << std::endl;
					continue;
				}
			}

			return 1;
		}

		//Connect to local host
		else if (choice == "2")
		{
			ID = 2;

			opponentIP = "127.0.0.1";
			opponentPort = 54444;
			if (!InitClient())
			{
				std::cout << "Error connecting to host, try again" << std::endl;
				continue;
			}
			return 2;
		}

		//Any other number will be taken as the IP address of a host to search for
		else
		{
			ID = 2;
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
		}

	}
}

bool ConnectionHandler::InitHost()
{
	// bind the socket to a port
	if (socket->bind(opponentPort) != sf::Socket::Done)
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
	if (socket->bind(sf::Socket::AnyPort) != sf::Socket::Done)
	{
		// error...
		std::cout << "Bind Failed" << std::endl;
	}

	//Tries 10,000 times to connect to host
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



		opponentIP = "127.0.0.1";
		opponentPort = 54444;

		if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
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


		if (socket->receive(packet, opponentIP, opponentPort) != sf::Socket::Done)
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
	if (ID == 1)
	{
		//start game if host presses space
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			std::string s;
			if (rollback)
			{
				s = "rollback";
			}
			else
			{
				s = "delay";
			}
			sf::Packet packet;
			packet << s;

			if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
			{
				// error...
				//send failed try it again
				std::cout << "Send begin failed, try again" << std::endl;
				return false;
			}

			//socket->unbind();
			return true;
		}
		sf::IpAddress senderIP;
		unsigned short senderPort;
		sf::Packet packet;
		std::string greeting;

		if (socket->receive(packet, senderIP, senderPort) != sf::Socket::Done)
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
				if (socket->send(packet, opponentIP, opponentPort) != sf::Socket::Done)
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
		if (socket->receive(packet, hostIP, hostPort) != sf::Socket::Done)
		{
			// error...
			//recieve failed send hello again
			//std::cout << "no messages yet" << std::endl;
			return false;
		}

		packet >> beginMessage;
		if (beginMessage == "rollback" || beginMessage == "delay")
		{
			if (beginMessage == "rollback")
			{
				rollback = true;
			}
			else
			{
				rollback = false;
			}
			return true;
		}
	}
	return false;
}


sf::IpAddress ConnectionHandler::GetOpponentIP()
{
	return opponentIP;
}
unsigned short ConnectionHandler::GetOpponentPort()
{
	return opponentPort;
}

unsigned short ConnectionHandler::GetOwnPort()
{
	return socket->getLocalPort();
}

sf::UdpSocket* ConnectionHandler::GetSocket()
{
	return socket;
}

bool ConnectionHandler::IsRollBackOn()
{
	return rollback;
}

int ConnectionHandler::GetLocalPlayerNumber()
{
	return ID - 1;
}

