// NetFight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Fighter.h"
#include "Input.h"
#include "HealthBar.h"
#include "GameStateManager.h"

#define FPS 60

GameStateManager stateManager;
GameState currentState;

Fighter* player1;
Fighter* player2;

HealthBar* healthBar1;
HealthBar* healthBar2;


Input inputHandler;

int screenWidth = 1000;
int screenHeight = 600;

sf::Time timeFromClock;
sf::Time frameTime;
float timeUntilFrameUpdate;

int frameCount = 0;
int main()
{
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Fight");
	window.setKeyRepeatEnabled(false);
	//window.setFramerateLimit(FPS);

	player1 = new Fighter(sf::Vector2f(200.0f, 350.0f), 1, screenWidth);
	player2 = new Fighter(sf::Vector2f(700.0f, 350.0f), 2, screenWidth);

	healthBar1 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(75, 30));
	healthBar2 = new HealthBar(sf::Vector2f(375.0f, 50.0f), sf::Vector2f(550, 30));

	timeUntilFrameUpdate = 1.0f / 60.0f;

	sf::Clock frameClock;
	

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
			frameCount++;
		}
		//std::cout << "Frametime is: " << frameTime.asSeconds() << std::endl;

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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
