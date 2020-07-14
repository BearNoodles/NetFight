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
#include "Menu.h"

#define FPS 60
//#define FRAME_DELAY 2

int rollbackFrameCount;

int framesInSecondMax = 60;
int roundTimerInitial = 99;

GameStateManager stateManager;
GameState currentState;
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
sf::Text delayText;

int delayLimit = 99;

sf::Text frameText;

sf::Time timeFromClock;
sf::Time frameTime;
sf::Time timeUntilFrameUpdate;
sf::Time timeSinceLastFrame = sf::Time::Zero;

int frameCount = 0;
sf::Clock frameClock;
sf::Clock pingClock;
float pingMaxWait = 10000.0f;
sf::Time ping;

void RunFrame();

void Restart();

void DrawCurrentFrame();
void AdvanceFrame(int frame);

bool HandleInputs();
void SetLocalInputs();
void SendInputs();
void UpdateInputs();
void ReadInputs(int frame);

ConnectionHandler connectionHandler;

MessageHandler messageHandler;

bool rollbackOn;
bool hitboxesOn;

int floorHeight = 500;
sf::Vector2i p1StartPos(200, floorHeight);
sf::Vector2i p2StartPos(700, floorHeight);

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");

bool gameFinished = false;

enum PlayState{menu, playing};
PlayState state = menu;

Menu* menuScreen;

//Called at the start of the game and aftre each round to reset everything
void Restart()
{
	//Bool to check if the game has ended or not
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

	currentDelay = 0;

	frameClock.restart();
	pingClock.restart();

	sf::Time timeSinceLastFrame = sf::Time::Zero;
	timeFromClock = sf::Time::Zero;
	frameTime = sf::Time::Zero;

	ping = sf::Time::Zero;
}

int main()
{
	menuScreen = new Menu(&window);
	//Start with menu screen
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

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
			{
				hitboxesOn = !hitboxesOn;
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
			{
				gameFinished = true;;
			}

			//Focus is used to stop inputs if the window is not in focus
			//Had to be turned off for local testing to allow both windows to be controlled at once
			if (event.type == sf::Event::LostFocus)
				//focus = false;

				if (event.type == sf::Event::GainedFocus)
					focus = true;
		}

		menuScreen->UpdateMenu();


		window.clear(sf::Color::Yellow);

		menuScreen->DrawMenu();

		window.display();
	}



	//Gets the data to represent the chosen character
	charData = new CharacterData();

	hitboxesOn = false;

	//Initialize some variables
	rollbackFrameCount = 0;

	rollbackOn = false;

	framesInSecondMax = 60;

	roundTimerInitial = 99;

	//set 1 frame equal to 1/60th of a second
	timeUntilFrameUpdate = sf::seconds(1.0f / 60.0f);

	//load and set font
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

	//Set up required texts
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

	window.setKeyRepeatEnabled(false);

	//Initialise the 2 player objects
	player1 = new Fighter(p1StartPos, 1, screenWidth, floorHeight);
	player2 = new Fighter(p2StartPos, 2, screenWidth, floorHeight);

	//Set the player characters (currently only 1 character)
	player1->SetCharacterData(charData->LoadCharacter1());
	player2->SetCharacterData(charData->LoadCharacter1());

	//Set up health bars
	healthBar1 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(75, 30));
	healthBar2 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(550, 30));

	//set up window display and get host and client connections
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

	//set the networking type, false for delay
	rollbackOn = connectionHandler.IsRollBackOn();

	//Pass local player number to the input handler
	inputHandler.SetPlayerNumber(connectionHandler.GetLocalPlayerNumber());

	//Initialise message handler
	messageHandler.Initialise(connectionHandler.GetOpponentIP(), connectionHandler.GetOpponentPort(), connectionHandler.GetSocket());

	//Start the clock for timing the frames
	frameClock.restart();
	
	//Set the round timer count and initialise how many frames are in each second
	currentState.roundTimer = roundTimerInitial;
	currentState.framesInSecond = 0;

	//Restart the timer which measures ping
	pingClock.restart();

	//Main game window loop
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

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
			{
				hitboxesOn = !hitboxesOn;
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
			{
				gameFinished = true;;
			}

			//Focus is used to stop inputs if the window is not in focus
			//Had to be turned off for local testing to allow both windows to be controlled at once
			if (event.type == sf::Event::LostFocus)
				//focus = false;

			if (event.type == sf::Event::GainedFocus)
				focus = true;
		}

		//Skipped if using delay networking		
		if (rollbackOn)
		{
			//Checks if theres is a rollback required and to which frame the game should be rolled back to
			int rollbackFrame = messageHandler.ReceiveMessagesRollback(frameCount);
			if (rollbackFrame != -1)
			{
				//std::cout << "Frame; " << frameCount << "	Rollback: " << rollbackFrame << std::endl;

				int step = rollbackFrame - 1;

				//Resets the game to the frame before the rollback frame, since thisis the last correct frame
				//and then steps forward again to the rollback frame
				player2->SetFighterState(stateManager.GetState(step));
				player1->SetFighterState(stateManager.GetState(step));

				currentState.frame = stateManager.GetState(step).frame;
				currentState.framesInSecond= stateManager.GetState(step).framesInSecond;
				currentState.roundTimer = stateManager.GetState(step).roundTimer;
				step++;

				//Simulates each frame with the noew inputs without drawing
				while (step < frameCount)
				{
					UpdateInputs();
					ReadInputs(step);
					AdvanceFrame(step);
					step++; 
				}
			}
		}
		//Receives messages without checking for rollbacks
		else
		{
			messageHandler.ReceiveMessagesDelay();
		}

		//Used in delay networking to set the current ping
		if (!rollbackOn && messageHandler.CheckPing())
		{
			ping = pingClock.restart();
			if ((ping.asSeconds() * 60) + 1 < 99)
			{
				currentDelay = ((ping.asSeconds() * 60)/2) + 1;
			}
		}

		else if (pingClock.getElapsedTime().asMilliseconds() > pingMaxWait)
		{
			messageHandler.ResetPing();
		}

		else if(rollbackOn)
		{
			currentDelay = 0;
		}

		if (!gameFinished && messageHandler.GetRestartReceived())
		{
			gameFinished = true;
		}
		//Send restart request to opponent and restarts the same if the opponent has also sent a restart request
		if (gameFinished)
		{
			messageHandler.SendRestartMessage();
			if (messageHandler.GetRestartReceived())
			{
				Restart();
			}
			
			//Dont advance past this point if the game needs restarted, just send request again
			continue;
		}

		//timer to check if a new frame is ready
		timeSinceLastFrame += frameClock.restart();

		//Make sure both players inputs are unchangable at this point to give the network some time to send and receive
		//Runs frame when ready
		if (timeSinceLastFrame > timeUntilFrameUpdate)
		{
			timeSinceLastFrame -= timeUntilFrameUpdate;
			RunFrame();
		}
	}
}

void RunFrame()
{
	//If the opponent inputs for this frame arent ready yet, return and restart until they are
	//Inputs should always be ready for rollback
	if (!HandleInputs())
	{
		//Local inputs for frame cannot be changed anymore, since they have been sent to the opponent
		dontUpdateLocal = true;
		return;
	}

	dontUpdateLocal = false;
	
	//Advance frame forward by 1
	AdvanceFrame(frameCount);
	frameCount++;


	//Render all sprites and text to the window
	DrawCurrentFrame();

	//Text for the current delay amount in frames
	delayText.setString("delay: " + std::to_string(currentDelay));
}


bool HandleInputs()
{
	//Set local inputs if not already set
	if (!dontUpdateLocal)
	{
		SetLocalInputs();
	}

	//Send to opponent
	SendInputs();

	//Recieves and sets opponent inputs
	UpdateInputs();

	//If delay is on and either input is not ready
	if (!rollbackOn && !inputHandler.BothInputsReady(frameCount))
	{
		return false;
	}

	//Reads inputs and send to player objects
	ReadInputs(frameCount);

	return true;
}

void SetLocalInputs()
{
	inputHandler.SetCurrentFrame(frameCount);

	//Wont handler local inputs if window is not in focus, the current delay is too large or the game is withing the first 100 frames
	if (focus && currentDelay < delayLimit && frameCount >= 100)
	{
		inputHandler.SetLocalInput(inputHandler.ReadLocalInput(frameCount + currentDelay));
	}
	//Sets all input buttons as false
	else
	{
		inputHandler.SetLocalInput(inputHandler.GetNoInput(frameCount + currentDelay));
	}

	//Not a rollback prediction
	//Used for if theres is a spike in the delay causes some frames to miss inputs, and uses the nomral prediction to fill them
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
	//For rollback send the last 3 frames of inputs messages (increases if delay is used alongside rollback)
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

	//For delay send the last 3 plus current delay amount messages of input frames
	else
	{
		for (int i = 0; i < 3 + currentDelay; i++)
		{

			if (frameCount + currentDelay < i)
			{
				break;
			}
			inputHandler.ForceSet(frameCount - i + currentDelay);
			messageHandler.SendFrameInput(inputHandler.GetLocalInput(frameCount - i + currentDelay));
		}
	}
}


void UpdateInputs()
{
	int setInputLimit = 1;
	if (rollbackOn)
	{
		//Maybe set limit of furthest possible rollback at top
		//Stops the game rolling back to a non existent frame
		if (frameCount < 20)
		{
			setInputLimit = frameCount + 1;
		}
		else
		{
			setInputLimit = 20;
		}

		//Checks if the last 20 frames of the opponents inputs are correct
		for (int i = 0; i < setInputLimit; i++)
		{
			inputHandler.SetOpponentInput(messageHandler.GetFrameInput(frameCount - i));
		}
	}

	//For rollback only need to set the inputs for the current frame
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

		//Gets the inputs for local player
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

		//Gets the inputs for local player
		player2Input = inputHandler.GetLocalInput(frame);

		//Get received input
		player1Input = inputHandler.GetOpponentInput(frame);
	}

	//Sets both players inputs
	player1->SetInput(player1Input);
	player2->SetInput(player2Input);
}


void AdvanceFrame(int frame)
{
	//Checks if either player has been hit and handles the collisions
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

	//Checks if players are walking into each other
	if (player1->CheckPushing(player2->GetHurtbox()) && player2->IsCornered())
	{
		player1->WalkPush();
	}
	if (player2->CheckPushing(player1->GetHurtbox()) && player1->IsCornered())
	{
		player2->WalkPush();
	}

	//Update player logic
	player1->UpdateFrame();
	player2->UpdateFrame();

	//Sets the gamestate frame if needed
	if (rollbackOn)
	{
		currentState.frame = frame;
	}
	
	//Updates the round timer
	if (currentState.framesInSecond >= framesInSecondMax)
	{
		currentState.framesInSecond = 0;
		currentState.roundTimer--;
	}
	roundTimeText.setString(std::to_string(currentState.roundTimer));

	frameText.setString("frame: " + std::to_string(frame));

	currentState.framesInSecond++;

	//Saves the gamestate for rollbacks
	if (rollbackOn)
	{
		stateManager.CreateNewGameState(player1->GetFighterState(), player2->GetFighterState(), currentState);
	}

	//Ends the game if the timer is less than zero
	if (currentState.roundTimer < 0)
	{
		gameFinished = true;
	}

}



void DrawCurrentFrame()
{
	//Clears window
	window.clear(sf::Color::Blue);

	//Draws players and ther hurtboxes
	window.draw(player1->GetAnimationFrame());
	window.draw(player2->GetAnimationFrame());

	if (hitboxesOn)
	{
		window.draw(player1->GetHurtbox());
		window.draw(player2->GetHurtbox());

		//Only tries to draw player hitboxes if there is an active one
		if (player1->IsHitboxActive())
		{
			window.draw(player1->GetActiveHitbox());
		}
		if (player2->IsHitboxActive())
		{
			window.draw(player2->GetActiveHitbox());
		}
	}
	

	//Update and draw healthbars
	healthBar1->UpdateHealth(player1->GetHealth());
	healthBar2->UpdateHealth(player2->GetHealth());

	window.draw(healthBar1->GetHealthBarBack());
	window.draw(healthBar1->GetHealthBar());
	window.draw(healthBar2->GetHealthBarBack());
	window.draw(healthBar2->GetHealthBar());

	//Draw text
	window.draw(roundTimeText);
	window.draw(delayText);
	window.draw(frameText);

	window.display();
}


