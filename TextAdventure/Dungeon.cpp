#include "Dungeon.h"
#include "Enums.h"
#include "Random.h"
#include "Structs.h"

#include <cmath>
#include <iostream>
#include <set>

void Dungeon::generateRoomTypes(int numOfRooms)
{
	std::cout << "Generate room types\n";
	
	int gemShardRoomCount{ 4 };
	int enemyRoomCount{ static_cast<int>(std::floor(numOfRooms * .4)) };
	int potionRoomCount{ static_cast<int>(std::ceil(numOfRooms * .4) - gemShardRoomCount) }; // Want 40% of rooms to be "Item" rooms
	
	int currentIndex{ 0 };

	Room room{ RoomType::Start };
	m_rooms.insert({ room, std::map<char, int>{} });
	++currentIndex;

	std::cout << "Start room populated.\n";

	while (gemShardRoomCount > 0)
	{
		Room gemRoom{ RoomType::GemShard };
		m_rooms.insert({ gemRoom, std::map<char, int>{} });
		--gemShardRoomCount;
		++currentIndex;
	}

	std::cout << "Gem Shard rooms populated.\n";
	
	while (enemyRoomCount > 0)
	{
		Room enemyRoom{ RoomType::Enemy };
		m_rooms.insert({ enemyRoom, std::map<char, int>{} });
		--enemyRoomCount;
		++currentIndex;
	}

	std::cout << "Enemy rooms populated.\n";

	while (potionRoomCount > 0)
	{
		Room potionRoom{ RoomType::Potion };
		m_rooms.insert({ potionRoom, std::map<char, int>{} });
		--potionRoomCount;
		++currentIndex;
	}

	std::cout << "Potion rooms populated.\n";

	while (currentIndex < numOfRooms - 1)
	{
		Room emptyRoom{ };
		m_rooms.insert({ emptyRoom, std::map<char, int>{} });
		++currentIndex;
	}

	Room exitRoom{ RoomType::Exit };
	m_rooms.insert({ exitRoom, std::map<char, int>{} });

	std::cout << "Exit room populated.\n";
}

void Dungeon::generateAdjacentRooms()
{
	std::cout << "Generate Adjacent Rooms\n";

	
	for (auto entry : m_rooms)
	{
		int connectionAttempts{ 1 };
		bool shouldAddAnotherRoom{ true };
		Room currentRoom{ entry.first };
		std::map<char, int> adjRooms{ entry.second };

		while (connectionAttempts < 4 && shouldAddAnotherRoom) // Don't want this to go on forever
		{
			int minValue{ 1 };
			int maxValue{ m_rooms.size() };
			int roomIdToAdd{ Random::getRandomNumberInRange(minValue, maxValue) };
			auto room{ getRoomById(roomIdToAdd) };

			while (roomIdToAdd == currentRoom.id
				|| (room.first.type == RoomType::Exit && room.second.size() == 3)
				|| (room.first.type != RoomType::Exit && room.second.size() == 4)
				|| room.first.type == currentRoom.type)
			{
				roomIdToAdd = Random::getRandomNumberInRange(minValue, maxValue);
				room = getRoomById(roomIdToAdd);
			}

			std::vector<char> directions{ getOpenDirections(adjRooms, room.second) };

			std::pair<char, int> roomToAdd{};

			if (directions.size() == 0)
			{
				++connectionAttempts;
				std::cout << "No directions available, trying again. Current attempt: " << connectionAttempts << '\n';
				continue;
			}
			else if (directions.size() == 1)
			{
				roomToAdd.first = directions[0];
				roomToAdd.second = roomIdToAdd;
			}
			else
			{
				int randomDirection{ Random::getRandomNumberInRange(0, static_cast<int>(directions.size()) - 1) };

				roomToAdd.first = directions[randomDirection];
				roomToAdd.second = roomIdToAdd;
			}

			std::cout << "Adding room " << roomToAdd.second << " in direction " << roomToAdd.first << '\n';

			adjRooms.insert(roomToAdd);

			auto addedRoom{ getRoomById(roomIdToAdd) };

			if (roomToAdd.first == 'W' || roomToAdd.first == 'E')
			{
				auto northIndex{ adjRooms.find('N') };
				auto southIndex{ adjRooms.find('S') };

				if (northIndex != adjRooms.end())
				{
					std::map<char, int> adjacentRooms{ getAdjacentRooms(northIndex->second) };

					auto newNeighbor{ adjacentRooms.find(roomToAdd.first) };
					if (newNeighbor != adjacentRooms.end())
					{
						addedRoom.second.insert({ 'N', newNeighbor->second });
					}
				}

				if (southIndex != adjRooms.end())
				{
					std::map<char, int> adjacentRooms{ getAdjacentRooms(southIndex->second) };

					auto newNeighbor{ adjacentRooms.find(roomToAdd.first) };
					if (newNeighbor != adjacentRooms.end())
					{
						addedRoom.second.insert({ 'S', newNeighbor->second });
					}
				}
			}
			else
			{
				auto westIndex{ adjRooms.find('W') };
				auto eastIndex{ adjRooms.find('E') };

				if (westIndex != adjRooms.end())
				{
					std::map<char, int> adjacentRooms{ getAdjacentRooms(westIndex->second) };

					auto newNeighbor{ adjacentRooms.find(roomToAdd.first) };
					if (newNeighbor != adjacentRooms.end())
					{
						addedRoom.second.insert({ 'W', newNeighbor->second });
					}
				}

				if (eastIndex != adjRooms.end())
				{
					std::map<char, int> adjacentRooms{ getAdjacentRooms(eastIndex->second) };

					auto newNeighbor{ adjacentRooms.find(roomToAdd.first) };
					if (newNeighbor != adjacentRooms.end())
					{
						addedRoom.second.insert({ 'E', newNeighbor->second });
					}
				}
			}

			switch (roomToAdd.first)
			{
			case 'N':
				addedRoom.second.insert({ 'S', currentRoom.id });
				break;
			case 'S':
				addedRoom.second.insert({ 'N', currentRoom.id });
				break;
			case 'W':
				addedRoom.second.insert({ 'W', currentRoom.id });
				break;
			case 'E':
				addedRoom.second.insert({ 'E', currentRoom.id });
				break;
			default:
				std::cerr << "Error, unknown direction passed.\n";
				return;
			}
			
			if (currentRoom.type != RoomType::Exit)
			{
				if (adjRooms.size() < 4)
				{
					shouldAddAnotherRoom = Random::getRandomNumberInRange(0, 1);
				}
			}
			else
			{
				if (adjRooms.size() < 3)
				{
					shouldAddAnotherRoom = Random::getRandomNumberInRange(0, 1);
				}
			}
		}
	}
}

std::map<char, int> Dungeon::getAdjacentRooms(int roomId)
{
	std::cout << "Get adjacent rooms of room " << roomId << '\n';
	return std::map<char, int>{};
}

std::vector<char> Dungeon::getOpenDirections(std::map<char, int> currentRoom, std::map<char, int> adjRoom)
{
	std::cout << "Getting open directions of both currentRoom and adjRoom\n";
	return std::vector{ 'N' };
}

std::pair<Room, std::map<char, int>> Dungeon::getRoomById(int roomId)
{
	std::cout << "Getting room " << roomId << '\n';
	Room room{};
	return { room, std::set<char, int>{} };
}

void Dungeon::printMap()
{
	std::cout << "Printing map\n";

	for (const auto entry : m_rooms)
	{
		std::cout << "Room ID: " << entry.first.id << '\n';
		std::cout << "Room Type: " << Enums::printRoomType(entry.first.type) << '\n';
		std::cout << "Adjacent Rooms: ";

		for (const auto room : entry.second)
		{
			std::cout << room.second << room.first << ' ';
		}
		std::cout << '\n';
	}
}