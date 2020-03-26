// NetFight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Network.hpp>
#include <list>
#include <string>
#include "Fighter.h"
#include "Input.h"
#include "HealthBar.h"
#include "GameStateManager.h"
#include "Message.h"
#include "MessageHandler.h"
#include "MessageHandlerRollback.h"
#include "ConnectionHandler.h"
//#include "ggponet.h"
//#include "nongamestate.h"

#define FPS 60
#define FRAME_DELAY 2

int gameFrames, rollbackFrameCount;

int myID;

int framesInSecondMax = 60;
int roundTimerInitial = 99;

GameStateManager stateManager;
GameState currentState;

//NonGameState ngs = { 0 };

//GGPOPlayer *players;
Fighter* player1;
Fighter* player2;

CharacterData* charData;

int thisPlayer;

bool focus = true;

bool dontUpdateLocal = false;

FrameInput player1Input;
FrameInput player2Input;

HealthBar* healthBar1;
HealthBar* healthBar2;


Input inputHandler;

int screenWidth = 1000;
int screenHeight = 600;

sf::Text roundTimeText;
sf::Font timerFont;

int currentDelay;
//int nextDelay;
sf::Text delayText;

int delayLimit = 99;

sf::Text frameText;


sf::Time timeFromClock;
sf::Time frameTime;
//float timeUntilFrameUpdate;
sf::Time timeUntilFrameUpdate;
sf::Time timeSinceLastFrame = sf::Time::Zero;

bool sButt = true;

int frameCount = 0;
sf::Clock frameClock;

sf::Clock pingClock;
sf::Time ping;

//sf::Time messageTimer;
//bool msgReady;

void RunFrameDelay();

void Restart();

//void Init(int localport, int num_players, GGPOPlayer *players, int num_spectators);
bool BeginGame();
//void InitSpectator(int localport, int num_players, char *host_ip, int host_port);
void DrawCurrentFrame();
//void AdvanceFrame(int inputs[], int disconnect_flags);
void AdvanceFrame(int frame);
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
void ReadInputs(int frame);

//std::list<Message> messages;

ConnectionHandler connectionHandler;

//TODO: make delay and rollback message handler classes to inherit from the main one
MessageHandler messageHandler;

//MessageHandlerRollback messageRollback;

bool rollbackOn;

int floorHeight = 500;
sf::Vector2i p1StartPos(200, floorHeight);
sf::Vector2i p2StartPos(700, floorHeight);

//GGPOSession *ggpo = NULL;

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");

bool gameFinished = false;

void Restart()
{
	gameFinished = false;

	messageHandler.Reset();
	
	player1->Reset();
	player2->Reset();

	stateManager.Reset();
	currentState.frame = 0;
	currentState.framesInSecond = 0;
	currentState.roundTimer = roundTimerInitial;
	frameCount = 0;

	inputHandler.Reset();


	frameClock.restart();
	pingClock.restart();

	sf::Time timeSinceLastFrame = sf::Time::Zero;
	timeFromClock = sf::Time::Zero;
	frameTime = sf::Time::Zero;
	timeUntilFrameUpdate = sf::Time::Zero;
	ping = sf::Time::Zero;
}

int main()
{

	charData = new CharacterData();

	gameFrames = 0;
	rollbackFrameCount = 0;

	rollbackOn = false;

	framesInSecondMax = 60;

	roundTimerInitial = 99;



	timeUntilFrameUpdate = sf::seconds(1.0f / 60.0f);


	if (!timerFont.loadFromFile("font.ttf"))
	{
		// error...
		std::cout << "Load font failed" << std::endl;
	}

	else
	{
		roundTimeText.setFont(timerFont);
		delayText.setFont(timerFont);
		frameText.setFont(timerFont);
	}

	roundTimeText.setCharacterSize(56);
	roundTimeText.setFillColor(sf::Color::White);
	roundTimeText.setPosition(sf::Vector2f(470, 10));

	delayText.setCharacterSize(28);
	delayText.setFillColor(sf::Color::White);
	delayText.setPosition(sf::Vector2f(870, 100));

	frameText.setCharacterSize(28);
	frameText.setFillColor(sf::Color::White);
	frameText.setPosition(sf::Vector2f(50, 100));

	currentDelay = 0;

	//socket.setBlocking(false);

	//sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");
	window.setKeyRepeatEnabled(false);
	//window.setFramerateLimit(FPS);

	//charData = new CharacterData();

	player1 = new Fighter(p1StartPos, 1, screenWidth, floorHeight);
	player2 = new Fighter(p2StartPos, 2, screenWidth, floorHeight);

	player1->SetCharacterData(charData->LoadCharacter1());
	player2->SetCharacterData(charData->LoadCharacter1());

	healthBar1 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(75, 30));
	healthBar2 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(550, 30));

	while (window.isOpen())
	{
		window.clear(sf::Color::Blue);
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

	rollbackOn = connectionHandler.IsRollBackOn();


	//messageHandler.Initialise(connectionHandler.GetOpponentIP(), connectionHandler.GetOpponentPort(), connectionHandler.GetOwnPort());
	messageHandler.Initialise(connectionHandler.GetOpponentIP(), connectionHandler.GetOpponentPort(), connectionHandler.GetSocket());
	//messageRollback.Initialise(connectionHandler.GetOpponentIP(), connectionHandler.GetOpponentPort(), connectionHandler.GetSocket());


	frameClock.restart();
	
	currentState.roundTimer = roundTimerInitial;
	currentState.framesInSecond = 0;

	//CHECK MESSAGE HANDLER PREDICTION

	//stateManager.CreateNewGameState(player1->GetFighterState(), player2->GetFighterState(), currentState);

	pingClock.restart();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			/*if (event.type == sf::Event::Resized)
			{
				std::cout << "new width: " << event.size.width << std::endl;
				std::cout << "new height: " << event.size.height << std::endl;
			}*/

			if (event.type == sf::Event::LostFocus)
				focus = false;

			if (event.type == sf::Event::GainedFocus)
				focus = true;
		}


		/*std::cout << "Frames: " << gameFrames << std::endl;
		std::cout << "Rollback Frames: " << rollbackFrameCount << std::endl;*/

		
		if (rollbackOn)
		{
			int rollbackFrame = messageHandler.ReceiveMessagesRollback(frameCount);
			if (rollbackFrame != -1)
			{
				std::cout << "Frame; " << frameCount << "	Rollback: " << rollbackFrame << std::endl;
				int step = rollbackFrame - 1;

				//stateManager.TrimRolledbackStates(step);
				player2->SetFighterState(stateManager.GetState(step));
				player1->SetFighterState(stateManager.GetState(step));

				currentState.frame = stateManager.GetState(step).frame;
				currentState.framesInSecond= stateManager.GetState(step).framesInSecond;
				currentState.roundTimer = stateManager.GetState(step).roundTimer;
				step++;

				while (step < frameCount)
				{
					UpdateInputs();
					ReadInputs(step);
					AdvanceFrame(step);
					step++; 
				}
			}
		}
		else
		{
			messageHandler.ReceiveMessagesDelay();
		}

		if (!rollbackOn && messageHandler.CheckPing())
		{
			ping = pingClock.restart();
			currentDelay = (ping.asSeconds() * 60) + 1;
			std::cout << "ping is: " << currentDelay << std::endl;
		}

		else if(rollbackOn)
		{
			currentDelay = 0;
		}


		if (gameFinished)
		{
			messageHandler.SendRestartMessage();
			if (messageHandler.GetRestartReceived())
			{
				Restart();
			}
			//Check other players status
			continue;
		}

		timeSinceLastFrame += frameClock.restart();

		//Advance frame
		//TODO:
		//Make sure both players inputs are unchangable at this point to give the network some time to send and receive
		if (timeSinceLastFrame > timeUntilFrameUpdate)
		{
			timeSinceLastFrame -= timeUntilFrameUpdate;
			//timeSinceLastFrame = sf::Time::Zero;
			RunFrameDelay();
		}
		//std::cout << "Frametime is: " << frameTime.asSeconds() << std::endl;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
		{
			sButt = true;
		}
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && sButt && focus)
		{
			frameCount -= 20;
			stateManager.TrimRolledbackStates(frameCount);
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

void RunFrameDelay()
{
	if (!HandleInputs())
	{
		dontUpdateLocal = true;
		return;
	}

	dontUpdateLocal = false;
	
	gameFrames++;
	AdvanceFrame(frameCount);
	frameCount++;

	DrawCurrentFrame();

	delayText.setString("delay: " + std::to_string(currentDelay));
}

void RunFrameRollback()
{

}

bool HandleInputs()
{
	if (!dontUpdateLocal)
	{
		SetLocalInputs();
	}

	SendInputs();

	//Receive inputs here?

	UpdateInputs();

	//If delay is on and either input is not ready
	if (!rollbackOn && !inputHandler.BothInputsReady(frameCount))
	{
		return false;
	}

	ReadInputs(frameCount);

	return true;
}

void SetLocalInputs()
{
	inputHandler.SetCurrentFrame(frameCount);
	if (focus && currentDelay < delayLimit && frameCount >= 100)
	{
		inputHandler.SetLocalInput(inputHandler.ReadLocalInput(frameCount + currentDelay));
	}
	else
	{
		inputHandler.SetLocalInput(inputHandler.GetNoInput(frameCount + currentDelay));
	}

	if (!rollbackOn)
	{
		for (int i = frameCount; i < frameCount + currentDelay; i++)
		{
			inputHandler.PredictLocalInput(i);
		}
	}
}

void SendInputs()
{
	if (rollbackOn)
	{
		//how many previous messages to send
		for (int i = 0; i < 3 + currentDelay; i++)
		{
			int previousFrames = i;

			if (frameCount < i)
			{
				previousFrames = frameCount;
			}
			messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount - previousFrames + currentDelay));
		}
		
	}

	else
	{
		for (int i = 0; i < 3 + currentDelay; i++)
		{
			//int previousFrames = i;

			if (frameCount + currentDelay < i)
			{
				break;
			}
			inputHandler.ForceSet(frameCount - i + currentDelay);
			messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount - i + currentDelay));
		}
		//messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount));
	}
}


//TODO: fix this

//ONLY NEEDED FOR ROLLBACK, MAKE 1 FOR DELAY
void UpdateInputs()
{
	int setInputLimit = 1;
	//if (rollBackOn)
	if (rollbackOn)
	{
		//Maybe set limit of furthest possible rollback at top
		if (frameCount < 20)
		{
			setInputLimit = frameCount + 1;
		}
		else
		{
			setInputLimit = 20;
		}
		for (int i = 0; i < setInputLimit; i++)
		{
			inputHandler.SetOpponentInput(messageHandler.GetFrameInput(frameCount - i));
		}
	}
	else
	{
		for (int i = 0; i < setInputLimit; i++)
		{
			inputHandler.SetOpponentInput(messageHandler.GetFrameInput(frameCount - i));
		}
	}
}

void ReadInputs(int frame)
{
	inputHandler.SetCurrentFrame(frame);

	if (thisPlayer == 1)
	{
		/*if (focus)
		{
			player1Input = inputHandler.GetLocalInput(frame);
		}
		else
		{
			player1Input = inputHandler.GetNoInput(frame);
		}*/

		player1Input = inputHandler.GetLocalInput(frame);

		//Get received input
		player2Input = inputHandler.GetOpponentInput(frame);
	}

	else
	{
		/*if (focus)
		{
			player2Input = inputHandler.GetLocalInput(frame);
		}
		else
		{
			player2Input = inputHandler.GetNoInput(frame);
		}*/
		player2Input = inputHandler.GetLocalInput(frame);

		//Get received input
		player1Input = inputHandler.GetOpponentInput(frame);
	}

	player1->SetInput(player1Input);
	player2->SetInput(player2Input);
}

//TODO::  WHY DOES THIS SEEM TO BE RUNNING SEVERAL TIMES
////////////////////////////////////////////////////////
//MAYBE PUT IN A RUNNING COUNT OF ADVANCED FRAMES VS ROLLBACK FRAMES

//NOW WORK OUT WHY THEY ARE SLIGHTLY OUT OF SYNC

//ROLLBACK WORKING! (99%) FIX DELAY AGAIN
void AdvanceFrame(int frame)
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

	if (rollbackOn)
	{
		currentState.frame = frame;
	}
	

	if (currentState.framesInSecond >= framesInSecondMax)
	{
		currentState.framesInSecond = 0;
		currentState.roundTimer--;
	}
	roundTimeText.setString(std::to_string(currentState.roundTimer));

	frameText.setString("frame: " + std::to_string(frame));

	currentState.framesInSecond++;


	if (rollbackOn)
	{
		stateManager.CreateNewGameState(player1->GetFighterState(), player2->GetFighterState(), currentState);
	}

	if (currentState.roundTimer < 0)
	{
		gameFinished = true;
	}

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
	window.clear(sf::Color::Blue);



	

	window.draw(*player1->GetAnimationFrame());
	window.draw(*player2->GetAnimationFrame());
	window.draw(player1->GetHurtbox());
	window.draw(player2->GetHurtbox());

	if (player1->IsHitboxActive())
	{
		window.draw(player1->GetActiveHitbox());
	}
	if (player2->IsHitboxActive())
	{
		window.draw(player2->GetActiveHitbox());
	}

	healthBar1->UpdateHealth(player1->GetHealth());
	healthBar2->UpdateHealth(player2->GetHealth());

	window.draw(healthBar1->GetHealthBarBack());
	window.draw(healthBar1->GetHealthBar());
	window.draw(healthBar2->GetHealthBarBack());
	window.draw(healthBar2->GetHealthBar());

	window.draw(roundTimeText);
	window.draw(delayText);

	window.draw(frameText);



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

