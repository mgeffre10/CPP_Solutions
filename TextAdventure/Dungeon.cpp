#include "Dungeon.h"
#include "Enums.h"
#include "Random.h"
#include "Spider.h"
#include "Structs.h"

#include <cmath>
#include <iostream>
#include <map>

#include <set>


std::vector<Room> Dungeon::generateRoomTypes(int numOfRooms)
{
	std::vector<Room> rooms{};
	int gemShardRoomCount{ 4 };
	int enemyRoomCount{ static_cast<int>(std::floor(numOfRooms * .4)) };
	int potionRoomCount{ static_cast<int>(std::ceil(numOfRooms * .4) - gemShardRoomCount) }; // Want 40% of rooms to be "Item" rooms
	
	int currentIndex{ 0 };

	Room room{ RoomType::Start, currentIndex + 1 };
	room.bHasBeenVisited = true; // Player starts here
	rooms.push_back(room);
	++currentIndex;

	createRooms(gemShardRoomCount, RoomType::GemShard, currentIndex, rooms);
	createRooms(enemyRoomCount, RoomType::Enemy, currentIndex, rooms);
	createRooms(potionRoomCount, RoomType::Potion, currentIndex, rooms);

	while (currentIndex < numOfRooms - 1)
	{
		Room emptyRoom{ currentIndex + 1 };
		rooms.push_back(emptyRoom);
		++currentIndex;
	}

	Room exitRoom{ RoomType::Exit, currentIndex + 1 };
	rooms.push_back(exitRoom);

	return rooms;
}

void Dungeon::createRooms(int count, RoomType type, int &currentIndex, std::vector<Room> &rooms)
{
	while (count > 0)
	{
		Room room{ type, currentIndex + 1 };

		if (type == RoomType::Enemy)
		{
			Spider* spider{ new Spider(static_cast<Size>(Random::getRandomNumberInRange(0, 1))) };
			room.enemy = spider;
		}

		rooms.push_back(room);
		--count;
		++currentIndex;
	}
}

void Dungeon::generateAdjacentRooms(std::vector<Room> &rooms)
{
	m_rooms.push_back(rooms[0]);
	rooms.erase(rooms.begin());
	std::shuffle(rooms.begin(), rooms.end(), Random::mersenne);

	auto currentRoomIter{ m_rooms.begin() };
	int failureInjectionPoint{ 1 }; // Used if end of map reached but still have rooms to add
	
	while (static_cast<int>(rooms.size()) > 0 && currentRoomIter != m_rooms.end())
	{
		int currentRoomId{ currentRoomIter->id }; // Used later when iterator is invalidated by inserting room into dungeon

		auto beginIter{ rooms.begin() };
		int connectionCount{ getConnectionCount(*currentRoomIter) };
		
		for (int count{ connectionCount }; count > 0; --count)
		{
			if (beginIter == rooms.end()) break;
			auto tempIter{ beginIter };

			char direction{ getPotentialDirection(*currentRoomIter)};

			if (direction == ' ') break;
			Position newRoomPos{ determinePosition(currentRoomIter->position, direction) };

			auto potentialRoom{ roomAtPosition(newRoomPos) };

			if (potentialRoom != m_rooms.end())
			{
				if (!isValidPosition(*currentRoomIter, *potentialRoom)) continue;

				currentRoomIter->connectedRooms.insert({direction, potentialRoom->id});
				potentialRoom->connectedRooms.insert({getOppositeDirection(direction), currentRoomIter->id});
			}
			else
			{
				while (!isValidPosition(*currentRoomIter, *tempIter))
				{
					++tempIter;

					if (tempIter == rooms.end()) break;
				}

				if (tempIter != rooms.end())
				{
					tempIter->position = newRoomPos;
					currentRoomIter->connectedRooms.insert({ direction, tempIter->id });
					tempIter->connectedRooms.insert({ getOppositeDirection(direction), currentRoomIter->id });
					m_rooms.push_back(*tempIter);
					currentRoomIter = getRoomById(currentRoomId);
					if (tempIter == beginIter) beginIter = rooms.erase(tempIter);
					else rooms.erase(tempIter);
				}
			}
		}

		++currentRoomIter;
		if (currentRoomIter == m_rooms.end() && rooms.size() > 0) // Restart the process of connecting rooms
		{
			int connectedRoomCount{ 1 };
			auto roomIter{ std::find_if(getRoomById(failureInjectionPoint) + 1, m_rooms.end(), [connectedRoomCount](const Room &room) {return static_cast<int>(room.connectedRooms.size()) <= connectedRoomCount; }) };
			
			while (roomIter == m_rooms.end())
			{
				++connectedRoomCount;
				if (connectedRoomCount == 4) break;
				roomIter = std::find_if(getRoomById(failureInjectionPoint) + 1, m_rooms.end(), [connectedRoomCount](const Room &room) {return static_cast<int>(room.connectedRooms.size()) <= connectedRoomCount; });
			}
			
			if (roomIter == m_rooms.end())
			{
				std::cerr << "Unable to add all rooms, exiting.\n";
				exit(0);
			}

			currentRoomIter = roomIter;
			failureInjectionPoint = roomIter->id;
		}
	}
}

int Dungeon::getConnectionCount(const Room &room)
{
	if (room.type == RoomType::Start) return 3;

	int connectionCount{ static_cast<int>(room.connectedRooms.size()) };
	if (connectionCount > 3) return 0;

	return Random::getRandomNumberInRange(1, 2);
}

char Dungeon::getPotentialDirection(const Room &currentRoom)
{
	std::vector<char> openDirections{ getOpenDirections(currentRoom.connectedRooms) };

	if (openDirections.size() == 0) return ' ';
	
	if (static_cast<int>(openDirections.size()) == 1) return openDirections[0];
	else return openDirections[Random::getRandomNumberInRange(0, static_cast<int>(openDirections.size()) - 1)];
}

bool Dungeon::isValidPosition(const Room &currentRoom, const Room &adjRoom)
{
	if (currentRoom.type == RoomType::Start && adjRoom.type == RoomType::Exit) return false;
	else if (currentRoom.type == RoomType::Exit && adjRoom.type == RoomType::Start) return false;
	else if (adjRoom.type == RoomType::Exit && static_cast<int>(adjRoom.connectedRooms.size()) == 3) return false;
	else if (currentRoom.type != RoomType::Empty && currentRoom.type == adjRoom.type) return false;
	
	return true;
}

std::vector<Room>::iterator Dungeon::roomAtPosition(Position position)
{
	return std::find_if(m_rooms.begin(), m_rooms.end(), [position](const Room &room) { return room.position == position; });
}

std::vector<Room>::iterator Dungeon::getRoomById(int roomId)
{
	return std::find_if(m_rooms.begin(), m_rooms.end(), [roomId](const Room &room) {return room.id == roomId; });
}

char Dungeon::getOppositeDirection(char direction) const
{
	switch (direction)
	{
		case 'N': return 'S';
		case 'S': return 'N';
		case 'E': return 'W';
		case 'W': return 'E';
		default: return ' ';
	}
}

Position Dungeon::determinePosition(Position currentRoomPos, char direction)
{
	Position position{};

	switch (direction)
	{
		case 'N':
			position.xPos = currentRoomPos.xPos;
			position.yPos = currentRoomPos.yPos + 1;
			break;
		case 'S':
			position.xPos = currentRoomPos.xPos;
			position.yPos = currentRoomPos.yPos - 1;
			break;
		case 'E':
			position.xPos = currentRoomPos.xPos + 1;
			position.yPos = currentRoomPos.yPos;
			break;
		case 'W':
			position.xPos = currentRoomPos.xPos - 1;
			position.yPos = currentRoomPos.yPos;
			break;
		default:
			std::cerr << "Unknown direction, exiting.\n";
			return {};
	}

	return position;
}

std::vector<char> Dungeon::getOpenDirections(const std::map<char, int> &room)
{
	std::vector<char> openDirections{ 'N', 'S', 'E', 'W' };

	int currentRoomSize{ static_cast<int>(room.size()) };
	if (currentRoomSize > 0)
	{
		for (const auto entry : room)
		{
			auto iter{ std::find_if(openDirections.begin(), openDirections.end(), [entry](char value) { return value == entry.first; }) };

			if (iter != openDirections.end()) openDirections.erase(iter);
		}
	}

	return openDirections;
}

std::map<char, int> Dungeon::getConnectedRooms(int currentRoomId)
{
	return getRoomById(currentRoomId)->connectedRooms;
}

void Dungeon::printMap()
{
	std::cout << "Printing map\n";

	for (const auto &entry : m_rooms)
	{
		std::cout << "Room ID: " << entry.id << '\n';
		std::cout << "Room Type: " << Enums::printRoomType(entry.type) << '\n';
		std::cout << "Adjacent Rooms: ";

		for (const auto &room : entry.connectedRooms)
		{
			std::cout << room.second << room.first << ' ';
		}

		std::cout << '\n';
	}
}