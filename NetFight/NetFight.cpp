// NetFight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Network.hpp>
#include <list>
#include "Fighter.h"
#include "Input.h"
#include "HealthBar.h"
#include "GameStateManager.h"
#include "Message.h"
#include "ggponet.h"

#define FPS 60

int myID;

GameStateManager stateManager;
GameState currentState;

GGPOPlayer *players;
Fighter* player1;
Fighter* player2;

FrameInput player1Input;
FrameInput player2Input;

HealthBar* healthBar1;
HealthBar* healthBar2;


Input inputHandler;

int screenWidth = 1000;
int screenHeight = 600;

sf::Time timeFromClock;
sf::Time frameTime;
float timeUntilFrameUpdate;

bool s = true;

int frameCount = 0;
sf::Clock frameClock;

bool msgReady;

void RunFrame();
void HostOrClient(); 
bool InitHost();
bool InitClient();
bool WaitForPlayers();

void Init(int localport, int num_players, GGPOPlayer *players, int num_spectators);
bool BeginGame();
void InitSpectator(int localport, int num_players, char *host_ip, int host_port);
void VectorWar_DrawCurrentFrame();
void AdvanceFrame(int inputs[], int disconnect_flags);
//void VectorWar_RunFrame(HWND hwnd);
void Idle(int time);
void DisconnectPlayer(int player);
void Exit();

std::list<Message> messages;

sf::UdpSocket socket;
sf::IpAddress hostIP = "127.0.0.1";
unsigned short hostPort = 54444;

sf::IpAddress clientIP;
unsigned short clientPort;


GGPOSession *ggpo = NULL;

int main()
{
	socket.setBlocking(false);


	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");
	window.setKeyRepeatEnabled(false);
	//window.setFramerateLimit(FPS);

	player1 = new Fighter(sf::Vector2f(200.0f, 350.0f), 1, screenWidth);
	player2 = new Fighter(sf::Vector2f(700.0f, 350.0f), 2, screenWidth);

	healthBar1 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(75, 30));
	healthBar2 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(550, 30));

	timeUntilFrameUpdate = 1.0f / 60.0f;

	while (window.isOpen())
	{
		window.clear();
		window.display();
		//Setup or join a game
		HostOrClient();
		break;
	}

	while (true)
	{
		if (WaitForPlayers())
		{
			break;
		}
		window.clear();
		window.display();
	}

	

	msgReady = false;

	frameClock.restart();
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				std::cout << "new width: " << event.size.width << std::endl;
				std::cout << "new height: " << event.size.height << std::endl;
			}
		}


		frameTime = frameClock.getElapsedTime();

		//Advance frame
		if (frameTime.asSeconds() > timeUntilFrameUpdate)
		{
			//RESET FRAMECLOCK HERE?

			inputHandler.SetCurrentFrame(frameCount);
			inputHandler.UpdateInputs(frameCount);

			//MAKE THIS WORK (SO GGPO CAN BE PASSED (p1I, p2I, simframe))
			player1Input = inputHandler.GetInput(frameCount);
			player2Input = inputHandler.GetInput(frameCount);

			//UPDATE WITH PLAYER INPUTS
			RunFrame();
			/*frameClock.restart();

			inputHandler.SetCurrentFrame(frameCount);
			inputHandler.UpdateInputs(frameCount);

			player1->SetInput(inputHandler.GetInput(frameCount));

			bool player1Hit = player2->IsHitboxActive() && player1->CheckForHit(&player2->GetActiveHitbox());
			bool player2Hit = player1->IsHitboxActive() && player2->CheckForHit(&player1->GetActiveHitbox());
			if (player1Hit)
			{
				player2->HitLanded();
				player1->HandleCollision(player2->GetCurrentAction());
			}
			if (player2Hit)
			{
				player1->HitLanded();
				player2->HandleCollision(player1->GetCurrentAction());
			}

			if (player1->CheckPushing(player2->GetHurtbox()) && player2->IsCornered())
			{
				player1->WalkPush();
			}
			if (player2->CheckPushing(player1->GetHurtbox()) && player2->IsCornered())
			{
				player2->WalkPush();
			}

			player1->UpdateFrame();
			player2->UpdateFrame();

			currentState.frame = frameCount;
			currentState.time = 99;

			stateManager.CreateNewGameState(player1->GetFighterState(), player2->GetFighterState(), currentState);

			frameCount++;*/
		}
		//std::cout << "Frametime is: " << frameTime.asSeconds() << std::endl;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			s = true;
		}
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && s)
		{
			frameCount -= 20;
			stateManager.SetCurrentState(frameCount);
			player2->SetFighterState(stateManager.GetState(frameCount));
			player1->SetFighterState(stateManager.GetState(frameCount));
			
			//inputHandler.SetCurrentFrame(frameCount);
			//frameCount = stateManager.GetState(frameCount).frame;
			//time = stateManager.GetState(frameCount).time;
			s = false;
		}

		window.clear();


		window.draw(player1->GetHurtbox());
		window.draw(player2->GetHurtbox());
		if (player1->IsHitboxActive())
		{
			window.draw(player1->GetActiveHitbox());
		}

		healthBar1->UpdateHealth(player1->GetHealth());
		healthBar2->UpdateHealth(player2->GetHealth());

		window.draw(healthBar1->GetHealthBarBack());
		window.draw(healthBar1->GetHealthBar());
		window.draw(healthBar2->GetHealthBarBack());
		window.draw(healthBar2->GetHealthBar());

		window.display();

		/*if (frameCount == 100)
		{
			std::cout << "Final Position: " << player1->GetHurtbox().getPosition().x;
			std::cout << "Final Position: " << player1->GetHurtbox().getPosition().x;
		}*/
	}


	//return 0;
}

void RunFrame()
{
	frameClock.restart();

	inputHandler.SetCurrentFrame(frameCount);
	inputHandler.UpdateInputs(frameCount);

	player1->SetInput(inputHandler.GetInput(frameCount));

	bool player1Hit = player2->IsHitboxActive() && player1->CheckForHit(&player2->GetActiveHitbox());
	bool player2Hit = player1->IsHitboxActive() && player2->CheckForHit(&player1->GetActiveHitbox());
	if (player1Hit)
	{
		player2->HitLanded();
		player1->HandleCollision(player2->GetCurrentAction());
	}
	if (player2Hit)
	{
		player1->HitLanded();
		player2->HandleCollision(player1->GetCurrentAction());
	}

	if (player1->CheckPushing(player2->GetHurtbox()) && player2->IsCornered())
	{
		player1->WalkPush();
	}
	if (player2->CheckPushing(player1->GetHurtbox()) && player2->IsCornered())
	{
		player2->WalkPush();
	}

	player1->UpdateFrame();
	player2->UpdateFrame();

	currentState.frame = frameCount;
	currentState.time = 99;

	stateManager.CreateNewGameState(player1->GetFighterState(), player2->GetFighterState(), currentState);

	frameCount++;


	GGPOErrorCode result = GGPO_OK;
	int disconnect_flags;
	int inputs[2] = { 0 };

	if (ngs.local_player_handle != GGPO_INVALID_HANDLE) {
		int input = ReadInputs(hwnd);
#if defined(SYNC_TEST)
		input = rand(); // test: use random inputs to demonstrate sync testing
#endif
		result = ggpo_add_local_input(ggpo, ngs.local_player_handle, &input, sizeof(input));
	}

	// synchronize these inputs with ggpo.  If we have enough input to proceed
	// ggpo will modify the input list with the correct inputs to use and
	// return 1.
	if (GGPO_SUCCEEDED(result)) {
		result = ggpo_synchronize_input(ggpo, (void *)inputs, sizeof(int) * MAX_SHIPS, &disconnect_flags);
		if (GGPO_SUCCEEDED(result)) {
			// inputs[0] and inputs[1] contain the inputs for p1 and p2.  Advance
			// the game by 1 frame using those inputs.
			VectorWar_AdvanceFrame(inputs, disconnect_flags);
		}
	}
	VectorWar_DrawCurrentFrame();
}

void HostOrClient()
{

	while (true)
	{
		std::string choice;
		std::cout << "Type 1 to start a new game or enter IP of host (press 2 for default host)" << std::endl;
		std::cin >> choice;

		if (choice == "1")
		{
			if (!InitHost())
			{
				std::cout << "Error starting as host, try again" << std::endl;
				continue;
			}
			break;
		}
		else if (choice == "2")
		{
			if (!InitClient())
			{
				std::cout << "Error connecting to host, try again" << std::endl;
				continue;
			}
			break;
		}
		else
		{
			hostIP = choice;

			std::cout << "Enter port number of host" << std::endl;
			unsigned short portChoice;
			std::cin >> portChoice;
			hostPort = portChoice;
			if (!InitClient())
			{
				std::cout << "Error connecting to host, try again" << std::endl;
				continue;
			}
			break;
		}
	}
}

bool InitHost()
{

	// bind the socket to a port
	if (socket.bind(hostPort) != sf::Socket::Done)
	{
		return false;
	}


	myID = 1;
	//playerCount = myID;
	//sf::Vector2f startPos(playerCount * startPosX, startPosY);

	//INIT FIGHTER?
	//myPlayer.Init(texture, startPos, playerColours[playerCount], playerCount);
	return true;
}

bool InitClient()
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

		if (socket.send(packet, hostIP, hostPort) != sf::Socket::Done)
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
		if (socket.receive(packet, hostIP, hostPort) != sf::Socket::Done)
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
			myID = std::stoi(r);
			break;
		}
		else
		{
			std::cout << "Didnt receive number > 1, retrying" << std::endl;
		}
	}

	//sf::Vector2f startPos(playerCount * startPosX, startPosY);

	//myPlayer.Init(texture, startPos, playerColours[playerCount], playerCount);
	//startPos = sf::Vector2f((playerCount - 1) * startPosX, startPosY);
	//opponent.Init(texture, startPos, playerColours[playerCount - 1], playerCount - 1);

	return true;
}

bool WaitForPlayers()
{
	//TODO draw other players while waiting
	if (myID == 1)
	{
		//start game if host presses space
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			std::string s = "begin";
			sf::Packet packet;
			packet << s;

			if (socket.send(packet, clientIP, clientPort) != sf::Socket::Done)
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
			clientIP = senderIP;
			clientPort = senderPort;
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
				if (socket.send(packet, clientIP, clientPort) != sf::Socket::Done)
				{
					// error...
					//recieve failed send hello again
					std::cout << "playerCount send failed, rety" << std::endl;
					counter++;
					continue;
				}
				//playerCount++;
				//sf::Vector2f startPos(startPosX * playerCount, startPosY);
				//Player tempPlayer(texture, startPos, playerColours[playerCount], playerCount);
				//playerList.push_back(tempPlayer);

				//opponent.Init(texture, startPos, playerColours[playerCount], playerCount);
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

void Init(int localport, int num_players, GGPOPlayer *players, int num_spectators)
{
	GGPOErrorCode result;

	// Fill in a ggpo callbacks structure to pass to start_session.
	GGPOSessionCallbacks cb = { 0 };
	cb.begin_game = BeginGameCallback;
	cb.advance_frame = AdvanceFrameCallback;
	cb.load_game_state = vw_load_game_state_callback;
	cb.save_game_state = vw_save_game_state_callback;
	cb.free_buffer = vw_free_buffer;
	cb.on_event = vw_on_event_callback;
	cb.log_game_state = vw_log_game_state;

	/* Start a new session */
	result = ggpo_start_session(&ggpo,         // the new session object
		&cb,           // our callbacks
		"test_app",    // application name
		2,             // 2 players
		sizeof(int),   // size of an input packet
		8001);         // our local udp port
}

bool __cdecl BeginGameCallback(const char *name)
{
	return true;
}

void InitSpectator(int localport, int num_players, char *host_ip, int host_port)
{

}

void VectorWar_DrawCurrentFrame()
{

}

bool __cdecl AdvanceFrameCallback(int inputs[], int disconnect_flags)
{
	int inputs[2] = { 0 };
	int disconnect_flags;

	// Make sure we fetch new inputs from GGPO and use those to update
	// the game state instead of reading from the keyboard.
	ggpo_synchronize_input(ggpo, (void *)inputs, sizeof(int) * 2, &disconnect_flags);
	AdvanceFrame(inputs, disconnect_flags);
	return true;
}

void AdvanceFrame(int inputs[], int disconnect_flags)
{

}

void Idle(int time)
{

}

void DisconnectPlayer(int player)
{

}

void Exit()
{

}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
