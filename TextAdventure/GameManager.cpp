#include "GameManager.h"
#include "Random.h"

#include <algorithm>
#include <iostream>
#include <sstream>

void GameManager::constructDungeon()
{
	std::cout << "Constructing Dungeon...\n";

	m_dungeonMap = Dungeon{ Random::getRandomNumberInRange(20, 40) }; // Update with the random number
	//m_dungeonMap.printMap();
}

void GameManager::intro()
{
	std::cout << "Printing intro to user.\n";
}

GameStatus GameManager::gameLoop()
{
	std::cout << "Starting gameLoop.\n";

	CombatStatus combatStatus{};

	while (true)
	{
		bool bDidCombatOccur{ false };
		//m_dungeonMap.getAdjacentRooms(m_Player.getCurrentRoom());
		std::cout << "Display directions here.\n";

		std::stringstream userInput{};
		std::string iStreamString{};

		while (userInput.str().empty())
		{
			std::cout << "Enter in a direction from above list or type a command (type '-help' for list of commands): ";
			std::getline(std::cin, iStreamString);
			userInput << parseInput(iStreamString);
		}

		std::string input{ userInput.str() };
		
		std::cin.clear();

		if (input[0] == 'N' || input[0] == 'S' || input[0] == 'E' || input[0] == 'W') // update to look at valid directions
		{
			m_Player.move(input[0]);
			combatStatus;
		}
		else
		{
			executeCommand(input);
		}

		if (bDidCombatOccur)
		{
			std::cout << "Combat Occurred.\n";
			std::cout << "Check CombatStatus.\n";
		}
	}

	return GameStatus::PlayerExited;
}

CombatStatus GameManager::combatLoop(Enemy &enemy)
{
	std::cout << "Combat occurred.\n";
	enemy.getHealth();
	return CombatStatus::PlayerFled;
	
}

std::string GameManager::parseInput(const std::string &input)
{
	if (input.length() > 0)
	{
		int firstCharIndex{ static_cast<int>(input.find_first_not_of(' ')) };

		if (firstCharIndex != std::string::npos)
		{
			char firstChar{ input[firstCharIndex] };

			if ((firstChar == 'N' || firstChar == 'n') || (firstChar == 'S' || firstChar == 's')
				|| (firstChar == 'W' || firstChar == 'w') || (firstChar == 'E' || firstChar == 'e'))
			{
				return { static_cast<char>(std::toupper(firstChar)) };
			}
			else if (firstChar == '-')
			{
				int startIndex{ firstCharIndex };
				std::stringstream command{};
				command << input[startIndex++];

				while (std::isalpha(input[startIndex]))
				{
					command << input[startIndex++];
				}

				if (command.str().length() > 1)
				{
					return command.str();
				}
			}
		}
	}

	std::cout << "Invalid input, please try again.\n";
	return "";
}

void GameManager::executeCommand(const std::string_view &command)
{
	std::cout << "Executing command: " << command << '\n';
}

void GameManager::printCommandList()
{
	std::cout << "Printing commandList\n";
}

void GameManager::outro(const GameStatus &gameStatus)
{
	std::cout << "Printing outro to user with game status: " << static_cast<int>(gameStatus) << '\n';
}