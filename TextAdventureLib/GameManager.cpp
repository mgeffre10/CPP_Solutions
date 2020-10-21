#include "GameManager.h"
#include "Random.h"

#include <algorithm>
#include <iostream>
#include <sstream>

std::map<std::string, std::string> GameManager::m_commandList = {
	{"-heal", "If player has a potion in their inventory and is not full health, consumes potion for +40 health."},
	{"-help", "Displays the list of available commands"},
	{"-location", "Displays the player's current room and surrounding rooms. If room has not been visited, room type is 'Unknown'."},
	{"-status", "Prints the player's health and inventory."},
	{"-quit", "Quits the game."}
};

void GameManager::intro()
{
	std::cout << "Welcome to Dungeon Escapist!\n";
	std::cout << "The object of the game is to, now stay with me here... escape the dungeon!\n";
	std::cout << "You do this by collecting all 4 Gem Shards that are located randomly throughout the dungeon.\n\n";

	std::cout << "You will start in a room and be given the available directions from that room (N/S/E/W)\n";
	std::cout << "You can either move in one of the available directions or execute a command to do things\n";
	std::cout << "like see your status or determine where you are in the map.\n";
	std::cout << "We strongly recommend you have a pen and paper in order to map out the dungeon.\n\n";

	std::cout << "Beware! Some rooms have enemies and you can either choose to attack them or flee. ";
	std::cout << "You could fail to flee which would allow the enemy to attack you before you are given the options again.\n";
	std::cout << "That should be it for instructions, good luck and God's speed, dungeoneer!\n\n";

	std::cout << "You awake and find yourself in an unfamiliar room with no idea how you got there.\n";
}

GameStatus GameManager::gameLoop()
{
	CombatStatus combatStatus{};

	while (true)
	{
		std::map<char, int> connectedRooms{ m_dungeonMap.getConnectedRooms(m_player.getCurrentRoom()) };

		std::cout << "Available Directions: ";

		for (const auto room : connectedRooms)
		{
			std::cout << room.first << ' ';
		}

		std::cout << "\n\n";

		std::stringstream userInput{};
		std::string iStreamString{};
		std::cin.clear();

		while (userInput.str().empty())
		{
			std::cout << "Enter in a direction from the above list or type a command (type '-help' for list of commands): ";
			std::getline(std::cin, iStreamString);
			userInput << parseInput(iStreamString);

			if (!userInput.str().empty())
			{
				if (userInput.str()[0] != '-' && connectedRooms.find(std::toupper(userInput.str()[0])) == connectedRooms.end())
				{
					std::cout << "Invalid direction entered, please try again.\n";
					userInput.str(std::string());
					userInput.clear();
				}
				else if (userInput.str()[0] == '-')
				{
					if (m_commandList.find(userInput.str()) == m_commandList.end())
					{
						std::cout << "Invalid command, please try again or type '-help' to view list of commands.\n";
						userInput.str(std::string());
						userInput.clear();
					}
				}
			}
		}

		std::string input{ userInput.str() };

		std::cin.clear();

		if (connectedRooms.find(input[0]) != connectedRooms.end())
		{
			m_player.move(input[0], connectedRooms.find(input[0])->second);

			auto roomIter{ m_dungeonMap.getRoomById(m_player.getCurrentRoom()) };

			std::cout << "You have entered room " << roomIter->id << ".\n";
			determineResponseToMovement(roomIter->id, roomIter->type);

			if (roomIter->type == RoomType::Enemy)
			{
				roomIter->enemy->printEntryDescription();
				roomIter->bHasBeenVisited = true;
				combatStatus = combatLoop(roomIter->enemy);

				if (combatStatus == CombatStatus::PlayerFled)
				{
					m_player.moveBack();
					std::cout << "You successfully flee and move back to room " << m_player.getCurrentRoom() << '\n';
				}
				else
				{
					if (combatStatus == CombatStatus::EnemyDied)
					{
						delete (roomIter->enemy);
						roomIter->enemy = nullptr;
						roomIter->type = RoomType::Empty;
					}
					else
					{
						m_player.goBackToStart();
						return GameStatus::PlayerDied;
					}
				}
			}
			else if (roomIter->type == RoomType::Exit)
			{
				if (m_player.getItemCount("Gem Shard") == 4)
				{
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
						std::cout << "You see an indentation of a circle with 4 distinct slots in the ceiling above you.\n";
						roomIter->bHasBeenVisited = true;
					}
				}
			}
			else
			{
				roomIter->bHasBeenVisited = true;
			}
		}
		else
		{
			executeCommand(input);
		}
	}

	return GameStatus::PlayerExited;
}

void GameManager::determineResponseToMovement(int roomId, RoomType roomType)
{
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
		m_player.addItem(Enums::printRoomType(roomType));
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

CombatStatus GameManager::combatLoop(Enemy* enemy)
{
	char input{};

	while (true)
	{
		std::cout << "Choose whether you would like to (A)ttack or (F)lee: ";
		std::cin >> input;

		while (std::toupper(input) != 'A' && std::toupper(input) != 'F')
		{
			std::cout << "Invalid input, please try again.\n";
			std::cin.clear();
			std::cin >> input;
		}

		std::cin.clear();
		std::cin.ignore(32767, '\n');

		if (std::toupper(input) == 'A')
		{
			int playerDamage{ m_player.calculateDamage() };
			enemy->takeDamage(playerDamage);
			std::cout << "\nYou deal " << playerDamage << " damage. Enemy's remaining health: " << enemy->getHealth() << '\n';

			if (enemy->getHealth() > 0)
			{
				int enemyDamage{ enemy->calculateDamage() };
				m_player.takeDamage(enemyDamage);
				std::cout << "The enemy deals " << enemyDamage << " damage. Your remaining health: " << m_player.getHealth() << "\n\n";

				if (m_player.getHealth() <= 0) return CombatStatus::PlayerDied;
			}
			else
			{
				std::cout << "Your enemy has been defeated!\n";
				return CombatStatus::EnemyDied;
			}
		}
		else
		{
			int fleeChance{ Random::getRandomNumberInRange(0, 100) };

			Spider* spider = dynamic_cast<Spider*>(enemy);

			if (spider->getSize() == Size::Small)
			{
				if (fleeChance > 70) break;
				else if (spider->getHealth() > spider->getMaxHealth() * .8 && fleeChance > 50) break;
				else if (spider->getHealth() > spider->getMaxHealth() * .6 && fleeChance > 30) break;
			}
			else
			{
				if (spider->getHealth() > spider->getMaxHealth() * .8 && fleeChance > 70) break;
				else if (spider->getHealth() > spider->getMaxHealth() * .6 && fleeChance > 50) break;
				else if (spider->getHealth() < spider->getMaxHealth() * .6 && fleeChance > 30) break;
			}

			std::cout << "\nYour flee failed! The enemy goes in for an attack.\n";

			int enemyDamage{ enemy->calculateDamage() };
			m_player.takeDamage(enemyDamage);
			std::cout << "The enemy deals " << enemyDamage << " damage. Your remaining health: " << m_player.getHealth() << "\n\n";

			if (m_player.getHealth() <= 0) return CombatStatus::PlayerDied;
		}
	}

	return CombatStatus::PlayerFled;
}

std::string GameManager::parseInput(const std::string& input)
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

void GameManager::executeCommand(const std::string_view& command)
{
	std::cout << "Executing command: " << command << '\n';

	if (command == "-status")
	{
		m_player.printStatus();
	}
	else if (command == "-heal")
	{
		if (!m_player.isFullHealth() && m_player.getItemCount("Potion") > 0)
		{
			m_player.heal(40);
			std::cout << "You heal for 40 health and now have " << m_player.getHealth() << " health.\n";
		}
		else if (m_player.isFullHealth()) std::cout << "You are already full health.\n";
		else if (m_player.getItemCount("Potion") == 0) std::cout << "You don't have any potions.\n";
	}
	else if (command == "-location")
	{
		auto currentRoom{ m_dungeonMap.getRoomById(m_player.getCurrentRoom()) };

		std::cout << "You are currently in room " << currentRoom->id << '\n';

		std::cout << "---------------Rooms surrounding you--------------\n";

		for (const auto room : currentRoom->connectedRooms)
		{
			auto roomIter{ m_dungeonMap.getRoomById(room.second) };
			std::string_view roomType{ roomIter->bHasBeenVisited ? Enums::printRoomType(roomIter->type) : "Unknown" };

			std::cout << "Room in the " << room.first << " direction: " << roomType << '\n';
		}

		std::cout << "--------------------------------------------------\n";
	}
	else if (command == "-help")
	{
		printCommandList();
	}
	else if (command == "-quit")
	{
		exit(0);
	}
}

void GameManager::printCommandList()
{
	std::cout << "--------------Command List--------------\n";

	for (const auto command : m_commandList)
	{
		std::cout << command.first << "\t" << command.second << "\n\n";
	}

	std::cout << "----------------------------------------\n";
}

void GameManager::outro(const GameStatus& gameStatus)
{
	if (gameStatus == GameStatus::PlayerDied)
	{
		std::cout << "You feel yourself slipping away into darkness. Things go black for a second and\nyou find yourself back at the start!\n";
		m_player.restoreHealth();
	}
	else
	{
		std::cout << "You succesfully exited the dungeon! Good job dungeoneer!\n";
	}
}

bool GameManager::playAgain()
{
	char input{};

	std::cout << "Would you like to play again? (Y)es or (N)o\n";
	std::cin >> input;

	while (std::toupper(input) != 'Y' && std::toupper(input) != 'N')
	{
		std::cout << "Invalid input, please try again.\n";
		std::cin.clear();
		std::cin >> input;
	}

	std::cin.clear();
	std::cin.ignore(32767, '\n');

	return std::toupper(input) == 'Y';
}

void GameManager::reset()
{
	m_dungeonMap.resetDungeon();
	m_player.restoreHealth();
	m_player.clearInventory();
	m_player.goBackToStart();
}