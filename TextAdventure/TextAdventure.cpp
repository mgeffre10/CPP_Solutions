// TextAdventure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Random.h"
#include "Enums.h"
#include "GameManager.h"

#include <iostream>

int main()
{
	GameManager manager{};
	bool playAgain{ true };
	while (playAgain)
	{
		manager.constructDungeon();
		manager.intro();
		GameStatus gameStatus{ GameStatus::PlayerDied };
		while (gameStatus == GameStatus::PlayerDied)
		{
			gameStatus = manager.gameLoop();
			manager.outro(gameStatus);
		}

		playAgain = manager.playAgain();

		if (playAgain)
		{
			manager.reset();
		}
	}
}