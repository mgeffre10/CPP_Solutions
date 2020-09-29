// TextAdventure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Random.h"
#include "Enums.h"
#include "GameManager.h"

#include <iostream>

int main()
{
	std::cout << "Inside main, starting Function flow.\n";
	GameManager manager{};
	manager.constructDungeon();
	manager.intro();
	GameStatus gameStatus{ manager.gameLoop() };
	manager.outro(gameStatus);
	std::cout << "Finished Function flow, game is over.\n";
}