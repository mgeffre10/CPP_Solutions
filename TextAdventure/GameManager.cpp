#include "GameManager.h"
#include "Random.h"

#include <algorithm>
#include <iostream>
#include <sstream>

void GameManager::constructDungeon()
{
	m_dungeonMap = Dungeon{ Random::getRandomNumberInRange(20, 40) };
}

void GameManager::intro()
{
	std::cout << "Printing intro to user.\n";
}

GameStatus GameManager::gameLoop()
{
	CombatStatus combatStatus{};

	while (true)
	{
		bool bDidCombatOccur{ false };
		std::map<char, int> connectedRooms{ m_dungeonMap.getConnectedRooms(m_Player.getCurrentRoom()) };
		
		std::cout << "Available Directions: ";

		for (const auto room : connectedRooms)
		{
			std::cout << room.first << ' ';
		}

		std::cout << "\n\n";

		std::stringstream userInput{};
		std::string iStreamString{};

		while (userInput.str().empty())
		{
			std::cout << "Enter in a direction from above list or type a command (type '-help' for list of commands): ";
			std::getline(std::cin, iStreamString);
			userInput << parseInput(iStreamString);

			if (userInput.str()[0] != '-' && connectedRooms.find(std::toupper(userInput.str()[0])) == connectedRooms.end())
			{
				std::cout << "Invalid direction entered, please try again.\n";
				userInput.str(std::string());
				userInput.clear();
			}
		}

		std::string input{ userInput.str() };
		
		std::cin.clear();

		if (connectedRooms.find(input[0]) != connectedRooms.end()) // update to look at valid directions
		{
			m_Player.move(input[0], connectedRooms.find(input[0])->second);

			auto roomIter{ m_dungeonMap.getRoomById(m_Player.getCurrentRoom()) };

			determineResponseToMovement(roomIter->id, roomIter->type);

			if (roomIter->type == RoomType::Enemy)
			{
				bDidCombatOccur = true;
				combatStatus = combatLoop(roomIter->enemy);
			}
			else if (roomIter->type == RoomType::Exit)
			{
				if (m_Player.getItemCount(Enums::printRoomType(roomIter->type)) == 4)
				{
					std::cout << "You escaped the dungeon!\n";
					return GameStatus::PlayerExited;
				}
				else
				{
					if (roomIter->bHasBeenVisited)
					{
						std::cout << "You still need some gem shards to exit the dungeon.\n";
					}
					else
					{
						std::cout << "You see an indentation of a circle with 4 distinct slots.\n";
					}
				}
			}
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

void GameManager::determineResponseToMovement(int roomId, RoomType roomType)
{
	std::cout << "You have entered room " << roomId << ".\n";

	switch (roomType)
	{
		case RoomType::Empty: 
			std::cout << "You see nothing interesting in this room.\n";
			return;
		case RoomType::Exit:				// Response handled in gameLoop
		case RoomType::Enemy: return;		// Response handled in gameLoop
		case RoomType::Potion:
		case RoomType::GemShard:
			std::cout << "You find a " << Enums::printRoomType(roomType) << " and pick it up!\n";
			m_Player.addItem(Enums::printRoomType(roomType));
			m_dungeonMap.getRoomById(roomId)->type = RoomType::Empty;
			return;
		case RoomType::Start:
			std::cout << "You seem to be back where you started!\n";
			return;
		default: 
			std::cout << "Invalid Room entered.\n";
			return;
	}

	return;
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