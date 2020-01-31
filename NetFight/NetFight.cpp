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
#include "MessageHandler.h"
#include "ConnectionHandler.h"
//#include "ggponet.h"
//#include "nongamestate.h"

#define FPS 60
#define FRAME_DELAY 2

int myID;

GameStateManager stateManager;
GameState currentState;

//NonGameState ngs = { 0 };

//GGPOPlayer *players;
Fighter* player1;
Fighter* player2;

int thisPlayer;

bool focus = true;

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

bool sButt = true;

int frameCount = 0;
sf::Clock frameClock;


sf::Time messageTimer;
bool msgReady;

void RunFrame();

//void Init(int localport, int num_players, GGPOPlayer *players, int num_spectators);
bool BeginGame();
//void InitSpectator(int localport, int num_players, char *host_ip, int host_port);
void DrawCurrentFrame();
//void AdvanceFrame(int inputs[], int disconnect_flags);
void AdvanceFrame();
//void VectorWar_RunFrame(HWND hwnd);
void Idle(int time);
void DisconnectPlayer(int player);
void Exit();


bool __cdecl BeginGameCallback(const char *name);

bool __cdecl AdvanceFrameCallback(int flags);

bool __cdecl LoadGameStateCallback(unsigned char *buffer, int len);

bool __cdecl SaveGameStateCallback(unsigned char **buffer, int *len, int *checksum, int frame);

void __cdecl FreeBuffer(void *buffer);

//bool __cdecl OnEventCallback(GGPOEvent *info);

bool __cdecl LogGameState(char *filename, unsigned char *buffer, int len);

int fletcher32_checksum(short *data, size_t len);

bool HandleInputs();
void SetLocalInputs();
void SendInputs();
void UpdateInputs();
void ReadInputs();

std::list<Message> messages;

ConnectionHandler connectionHandler;
MessageHandler messageHandler;

//sf::UdpSocket socket;
//sf::IpAddress hostIP = "127.0.0.1";
//unsigned short hostPort = 54444;
//
//sf::IpAddress clientIP;
//unsigned short clientPort;


//GGPOSession *ggpo = NULL;

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");


int main()
{
	//socket.setBlocking(false);


	//sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");
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
		thisPlayer = connectionHandler.HostOrClient();
		break;
	}


	while (true)
	{
		if (connectionHandler.WaitForPlayers())
		{
			break;
		}
		window.clear();
		window.display();
	}


	//messageHandler.Initialise(connectionHandler.GetOpponentIP(), connectionHandler.GetOpponentPort(), connectionHandler.GetOwnPort());
	messageHandler.Initialise(connectionHandler.GetOpponentIP(), connectionHandler.GetOpponentPort(), connectionHandler.GetSocket());

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

			if (event.type == sf::Event::LostFocus)
				focus = false;

			if (event.type == sf::Event::GainedFocus)
				focus = true;
		}



		//SEND MESSAGES



		frameTime = frameClock.getElapsedTime();

		//Advance frame
		if (frameTime.asSeconds() > timeUntilFrameUpdate)
		{
			RunFrame();
		}
		//std::cout << "Frametime is: " << frameTime.asSeconds() << std::endl;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			sButt = true;
		}
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sButt && focus)
		{
			frameCount -= 20;
			stateManager.SetCurrentState(frameCount);
			player2->SetFighterState(stateManager.GetState(frameCount));
			player1->SetFighterState(stateManager.GetState(frameCount));
			
			//inputHandler.SetCurrentFrame(frameCount);
			//frameCount = stateManager.GetState(frameCount).frame;
			//time = stateManager.GetState(frameCount).time;
			sButt = false;
		}

		/*window.clear();


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

		window.display();*/

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
	
//	GGPOErrorCode result = GGPO_OK;
//	int disconnect_flags;
//	int inputs[2] = { 0 };
//
//	if (currentState.localPlayerHandle != GGPO_INVALID_HANDLE) {
//		FrameInput input = ReadInputs(); // SET INPUTS
//#if defined(SYNC_TEST)
//		input = rand(); // test: use random inputs to demonstrate sync testing
//#endif
//		result = ggpo_add_local_input(ggpo, ngs.local_player_handle, &input, sizeof(input));
//	}
//
//	// synchronize these inputs with ggpo.  If we have enough input to proceed
//	// ggpo will modify the input list with the correct inputs to use and
//	// return 1.
//	if (GGPO_SUCCEEDED(result)) {
//		result = ggpo_synchronize_input(ggpo, (void *)inputs, sizeof(int) * 2, &disconnect_flags);
//		if (GGPO_SUCCEEDED(result)) {
//			// inputs[0] and inputs[1] contain the inputs for p1 and p2.  Advance
//			// the game by 1 frame using those inputs.
//			AdvanceFrame(inputs, disconnect_flags);
//		}
//	}
	if (!HandleInputs())
	{
		return;
	}
	//AdvanceFrame(inputs, disconnect_flags);
	AdvanceFrame();
	DrawCurrentFrame();
}

bool HandleInputs()
{
	SetLocalInputs();
	SendInputs();

	messageHandler.ReceiveInputMessages(frameCount);

	UpdateInputs();

	if (!inputHandler.BothInputsReady(frameCount))
	{
		return false;
	}

	ReadInputs();

	return true;
}

void SetLocalInputs()
{
	inputHandler.SetCurrentFrame(frameCount);
	if (focus)
	{
		inputHandler.SetLocalInput(inputHandler.ReadLocalInput(frameCount));
	}
	else
	{
		inputHandler.SetLocalInput(inputHandler.GetNoInput(frameCount));
	}

}

void SendInputs()
{
	//messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount - 2));
	//messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount - 1));
	messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount));
}


//TODO: fix this
void UpdateInputs()
{
	for (int i = 0; i < 10; i++)
	{
		inputHandler.SetOpponentInput(messageHandler.GetFrameInput(frameCount - i));
	}
}

void ReadInputs()
{
	inputHandler.SetCurrentFrame(frameCount);

	if (thisPlayer == 1)
	{
		if (focus)
		{
			player1Input = inputHandler.GetLocalInput(frameCount);
		}
		else
		{
			player1Input = inputHandler.GetNoInput(frameCount);
		}

		//Get received input
		player2Input = inputHandler.GetOpponentInput(frameCount);
	}

	else
	{
		if (focus)
		{
			player2Input = inputHandler.GetLocalInput(frameCount);
		}
		else
		{
			player2Input = inputHandler.GetNoInput(frameCount);
		}

		//Get received input
		player1Input = inputHandler.GetOpponentInput(frameCount);
	}

	player1->SetInput(player1Input);
	player2->SetInput(player2Input);
}


//void AdvanceFrame(int inputs[], int disconnect_flags)
void AdvanceFrame()
{
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

	// update the checksums to display in the top of the window.  this
	// helps to detect desyncs.
	//ngs.now.framenumber = currentState.frame;
	//ngs.now.checksum = fletcher32_checksum((short *)&currentState, sizeof(currentState) / 2);
	/*if ((currentState.frame % 90) == 0) {
		ngs.periodic = ngs.now;
	}*/

	// Notify ggpo that we've moved forward exactly 1 frame.
	//ggpo_advance_frame(ggpo);

	// Update the performance monitor display.
	//GGPOPlayerHandle handles[MAX_PLAYERS];
	/*int count = 0;
	for (int i = 0; i < ngs.num_players; i++) {
		if (ngs.players[i].type == GGPO_PLAYERTYPE_REMOTE) {
			handles[count++] = ngs.players[i].handle;
		}
	}*/
}



void DrawCurrentFrame()
{
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
}


void Exit()
{

}

//void Init(int localport, int num_players, GGPOPlayer *players, int num_spectators)
//{
//	GGPOErrorCode result;
//
//	// Fill in a ggpo callbacks structure to pass to start_session.
//	GGPOSessionCallbacks cb = { 0 };
//	cb.begin_game = BeginGameCallback;
//	cb.advance_frame = AdvanceFrameCallback;
//	cb.load_game_state = LoadGameStateCallback;
//	cb.save_game_state = SaveGameStateCallback;
//	cb.free_buffer = FreeBuffer;
//	cb.on_event = OnEventCallback;
//	cb.log_game_state = LogGameState;
//
//	/* Start a new session */
//	result = ggpo_start_session(&ggpo,         // the new session object
//		&cb,           // our callbacks
//		"NetFight",    // application name
//		2,             // 2 players
//		sizeof(int),   // size of an input packet
//		8001);         // our local udp port
//
//					   // automatically disconnect clients after 3000 ms and start our count-down timer
//					   // for disconnects after 1000 ms.   To completely disable disconnects, simply use
//					   // a value of 0 for ggpo_set_disconnect_timeout.
//	ggpo_set_disconnect_timeout(ggpo, 3000);
//	ggpo_set_disconnect_notify_start(ggpo, 1000);
//
//	int i;
//	for (i = 0; i < num_players + num_spectators; i++) {
//		GGPOPlayerHandle handle;
//		result = ggpo_add_player(ggpo, players + i, &handle);
//		ngs.players[i].handle = handle;
//		ngs.players[i].type = players[i].type;
//		if (players[i].type == GGPO_PLAYERTYPE_LOCAL) {
//			ngs.players[i].connect_progress = 100;
//			ngs.local_player_handle = handle;
//			ngs.SetConnectState(handle, Connecting);
//			ggpo_set_frame_delay(ggpo, handle, FRAME_DELAY);
//		}
//		else {
//			ngs.players[i].connect_progress = 0;
//		}
//	}
//}

//bool __cdecl AdvanceFrameCallback(int flags)
//{
//	int inputs[2] = { 0 };
//	int disconnect_flags;
//
//	// Make sure we fetch new inputs from GGPO and use those to update
//	// the game state instead of reading from the keyboard.
//	ggpo_synchronize_input(ggpo, (void *)inputs, sizeof(int) * 2, &disconnect_flags);
//	AdvanceFrame(inputs, disconnect_flags);
//	return true;
//}



//bool __cdecl OnEventCallback(GGPOEvent *info)
//{
//	int progress;
//	switch (info->code) {
//	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
//		ngs.SetConnectState(info->u.connected.player, Synchronizing);
//		break;
//	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
//		progress = 100 * info->u.synchronizing.count / info->u.synchronizing.total;
//		ngs.UpdateConnectProgress(info->u.synchronizing.player, progress);
//		break;
//	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
//		ngs.UpdateConnectProgress(info->u.synchronizing.player, 100);
//		break;
//	case GGPO_EVENTCODE_RUNNING:
//		ngs.SetConnectState(Running);
//		//renderer->SetStatusText("");
//		break;
//	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
//		//ngs.SetDisconnectTimeout(info->u.connection_interrupted.player,
//			//timeGetTime(),
//			//info->u.connection_interrupted.disconnect_timeout);
//		break;
//	case GGPO_EVENTCODE_CONNECTION_RESUMED:
//		ngs.SetConnectState(info->u.connection_resumed.player, Running);
//		break;
//	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
//		ngs.SetConnectState(info->u.disconnected.player, Disconnected);
//		break;
//	case GGPO_EVENTCODE_TIMESYNC:
//		//Sleep(1000 * info->u.timesync.frames_ahead / 60);
//		break;
//	}
//	return true;
//}





bool __cdecl BeginGameCallback(const char *name)
{
	return true;
}



bool __cdecl LogGameState(char *filename, unsigned char *buffer, int len)
{
	return true;
}

void Idle(int time)
{
	//ggpo_idle(ggpo, time);
}

void DisconnectPlayer(int player)
{

}

bool __cdecl LoadGameStateCallback(unsigned char *buffer, int len)
{
	memcpy(&currentState, buffer, len);
	return true;
}

bool __cdecl SaveGameStateCallback(unsigned char **buffer, int *len, int *checksum, int frame)
{
	*len = sizeof(currentState);
	*buffer = (unsigned char *)malloc(*len);
	if (!*buffer) {
		return false;
	}
	memcpy(*buffer, &currentState, *len);
	*checksum = fletcher32_checksum((short *)*buffer, *len / 2);
	return true;
}

void __cdecl FreeBuffer(void *buffer)
{
	free(buffer);
}

int fletcher32_checksum(short *data, size_t len)
{
	int sum1 = 0xffff, sum2 = 0xffff;

	while (len) {
		unsigned tlen = len > 360 ? 360 : len;
		len -= tlen;
		do {
			sum1 += *data++;
			sum2 += sum1;
		} while (--tlen);
		sum1 = (sum1 & 0xffff) + (sum1 >> 16);
		sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	}

	/* Second reduction step to reduce sums to 16 bits */
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	return sum2 << 16 | sum1;
}

