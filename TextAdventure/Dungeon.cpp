#include "Dungeon.h"
#include "Enums.h"
#include "Random.h"
#include "Structs.h"

#include <cmath>
#include <iostream>
#include <map>

#include <set>


std::vector<Room> Dungeon::generateRoomTypes(int numOfRooms)
{
	std::cout << "Generate room types\n";
	std::vector<Room> rooms{};
	int gemShardRoomCount{ 4 };
	int enemyRoomCount{ static_cast<int>(std::floor(numOfRooms * .4)) };
	int potionRoomCount{ static_cast<int>(std::ceil(numOfRooms * .4) - gemShardRoomCount) }; // Want 40% of rooms to be "Item" rooms
	
	int currentIndex{ 0 };

	Room room{ RoomType::Start };
	rooms.push_back(room);
	++currentIndex;

	std::cout << "Start room populated.\n";

	while (gemShardRoomCount > 0)
	{
		Room gemRoom{ RoomType::GemShard };
		rooms.push_back(gemRoom);
		--gemShardRoomCount;
		++currentIndex;
	}

	std::cout << "Gem Shard rooms populated.\n";
	
	while (enemyRoomCount > 0)
	{
		Room enemyRoom{ RoomType::Enemy };
		rooms.push_back(enemyRoom);
		--enemyRoomCount;
		++currentIndex;
	}

	std::cout << "Enemy rooms populated.\n";

	while (potionRoomCount > 0)
	{
		Room potionRoom{ RoomType::Potion };
		rooms.push_back(potionRoom);
		--potionRoomCount;
		++currentIndex;
	}

	std::cout << "Potion rooms populated.\n";

	while (currentIndex < numOfRooms - 1)
	{
		Room emptyRoom{ };
		rooms.push_back(emptyRoom);
		++currentIndex;
	}

	Room exitRoom{ RoomType::Exit };
	rooms.push_back(exitRoom);

	std::cout << "Exit room populated.\n";

	return rooms;
}

void Dungeon::generateAdjacentRooms(std::vector<Room> &rooms)
{
	std::cout << "Generate Adjacent Rooms\n";

	m_rooms.push_back(rooms[0]);
	rooms.erase(rooms.begin());
	std::shuffle(rooms.begin(), rooms.end(), Random::mersenne);

	std::cerr << "Rooms: ";

	for (const Room &room : rooms)
	{
		std::cerr << room.id << ' ';
	}

	std::cerr << '\n';

	auto currentRoomIter{ m_rooms.begin() };

	while (static_cast<int>(rooms.size()) > 0 && currentRoomIter != m_rooms.end())
	{
		int currentRoomId{ currentRoomIter->id }; // Used later when iterator is invalidated
		std::cerr << "Current Room in dungeon map: " << currentRoomId << '\n';

		auto beginIter{ rooms.begin() };
		int adjRoomCount{ getAdjRoomCount(*beginIter) };
		
		for (int count{ adjRoomCount }; count > 0; --count)
		{
			if (beginIter == rooms.end()) break;
			auto tempIter{ beginIter };
			char direction{ getPotentialDirection(*currentRoomIter) };

			if (direction == ' ')
			{
				std::cerr << "----------------------------------------Current room no longer has room.\n";
				break;
			}
			std::cerr << "Trying to add room " << tempIter->id << " to room " << currentRoomId << " in the direction " << direction << '\n';

			std::map<char, int> potentialNeighbors{ getNeighbors(currentRoomIter->position, direction) };

			std::cerr << "Potential Neighbors: ";
			for (const auto neighbor : potentialNeighbors)
			{
				std::cerr << neighbor.second << neighbor.first << ' ';
			}

			std::cerr << '\n';
			while (!isValidPosition(*tempIter, potentialNeighbors))
			{
				++tempIter;
				if (tempIter == rooms.end()) break;
				std::cerr << "Invalid room, moving to room " << tempIter->id << '\n';
			}

			if (tempIter != rooms.end())
			{
				Room roomToAdd{ *tempIter };
				roomToAdd.position = determinePosition(currentRoomIter->position, direction);
				roomToAdd.adjRooms = potentialNeighbors;

				m_rooms.push_back(roomToAdd);
				currentRoomIter = getRoomById(currentRoomId); // Iterator may have been invalidated
				std::cerr << "Added room " << roomToAdd.id << " to the dungeon map.\n";

				std::cerr << "Removing room " << tempIter->id << " from remaining rooms.\n";
				rooms.erase(tempIter);
				beginIter = rooms.begin(); // Iterator may have been invalidated

				propogateNewRoom(roomToAdd.id, potentialNeighbors);

			}
		}
		++currentRoomIter;
		std::cerr << "Moved to next room in dungeon map.\n";
		if (currentRoomIter == m_rooms.end()) std::cerr << "Reached end of dungeon map.\n";
	}

	printMap();
}

void Dungeon::generateConnections()
{
	for (auto roomIter{ m_rooms.begin() }; roomIter != m_rooms.end(); ++roomIter)
	{
		int adjRoomCount{ static_cast<int>(roomIter->adjRooms.size()) }; // 4
		std::cerr << "Adjacent Room Count: " << adjRoomCount << '\n';
		int connectionCount{ Random::getRandomNumberInRange(2, adjRoomCount) }; // 4
		int currentRoomId{ roomIter->id };

		std::cerr << "Removing " << adjRoomCount - connectionCount << " rooms from room " << currentRoomId << '\n';

		for (int i{ adjRoomCount }; i > connectionCount; --i)
		{
			std::cerr << "Trying to remove room.\n";
			// Get first adjacent room
			auto firstAdjacentRoomInCurrentRoom{ roomIter->adjRooms.begin() };
			// Get room object of adjacent room id
			int adjRoomId{ firstAdjacentRoomInCurrentRoom->second };
			std::cerr << "Room trying to remove: " << adjRoomId << '\n';
			auto firstAdjacentRoomInCurrentRoomObject{ getRoomById(adjRoomId) };
			

			auto tempIter{ firstAdjacentRoomInCurrentRoom };
			std::cerr << "Checking if we can remove room " << adjRoomId << '\n';
			while (static_cast<int>(firstAdjacentRoomInCurrentRoomObject->adjRooms.size()) <= 2)
			{
				std::cerr << "Adjacent room will no longer have any connections, moving to next adjacent room.\n";
				++tempIter;

				if (tempIter == roomIter->adjRooms.end()) break;
				adjRoomId = tempIter->second;
				firstAdjacentRoomInCurrentRoomObject = getRoomById(adjRoomId);
			}
			std::cerr << "Adjacent room is valid or tempIter is at end.\n";
			if (tempIter == roomIter->adjRooms.end()) break;

			std::cerr << "Deleting room " << adjRoomId << " from room " << currentRoomId << " and vice versa.\n";
			auto currentRoom{ std::find_if(firstAdjacentRoomInCurrentRoomObject->adjRooms.begin(), firstAdjacentRoomInCurrentRoomObject->adjRooms.end(), [currentRoomId](auto pair) {return currentRoomId == pair.second; }) };
			if (currentRoom != firstAdjacentRoomInCurrentRoomObject->adjRooms.end()) firstAdjacentRoomInCurrentRoomObject->adjRooms.erase(currentRoom);
			roomIter->adjRooms.erase(tempIter);

		}
	}

	printMap();
}

int Dungeon::getAdjRoomCount(const Room &room)
{
	int currentAdjRooms{ static_cast<int>(room.adjRooms.size()) };
	
	int randNumber{ Random::getRandomNumberInRange(0, 100) };


	if (randNumber > 90 && currentAdjRooms == 0) return 3;
	else if (randNumber > 80 && currentAdjRooms < 2) return 2;
	
	if (currentAdjRooms < 3) return 1;

	// based on random number and adj room count, return appropriate value
	return 0;
}

char Dungeon::getPotentialDirection(const Room &currentRoom)
{
	std::vector<char> openDirections{ getOpenDirections(currentRoom.adjRooms) };

	if (openDirections.size() == 0) return ' ';
	
	if (static_cast<int>(openDirections.size()) == 1) return openDirections[0];
	else return openDirections[Random::getRandomNumberInRange(0, static_cast<int>(openDirections.size()) - 1)];
}

bool Dungeon::isValidPosition(const Room &room, const std::map<char, int> &neighbors)
{
	for (const auto neighbor : neighbors)
	{
		auto neighborRoomIter{ getRoomById(neighbor.second) };
		if (room.type != RoomType::Empty && room.type == neighborRoomIter->type
			|| room.type == RoomType::Exit && neighborRoomIter->type == RoomType::Start
			|| neighborRoomIter->type == RoomType::Exit && static_cast<int>(neighborRoomIter->adjRooms.size()) == 3) return false;
	}

	return true;
}

void Dungeon::propogateNewRoom(int newRoomId, const std::map<char, int> neighbors)
{
	for (const auto neighbor : neighbors)
	{
		auto neighborIter{ getRoomById(neighbor.second) };

		if (neighborIter != m_rooms.end())
		{
			char direction{};
			switch (neighbor.first)
			{
			case 'N':
				direction = 'S';
				break;
			case 'S':
				direction = 'N';
				break;
			case 'E':
				direction = 'W';
				break;
			case 'W':
				direction = 'E';
				break;
			default: return;
			}

			std::cerr << "Adding room " << newRoomId << " to room " << neighborIter->id << " in the direction " << direction << '\n';
			neighborIter->adjRooms.insert({ direction, newRoomId });
		}
	}
}

std::vector<Room>::iterator Dungeon::getRoomById(int roomId)
{
	return std::find_if(m_rooms.begin(), m_rooms.end(), [roomId](const Room &room) {return room.id == roomId; });
}

std::map<char, int> Dungeon::getNeighbors(Position currentRoomPos, char roomToAddDirection)
{
	std::map<char, int> neighbors{};

	Position roomToAddPos{ determinePosition(currentRoomPos, roomToAddDirection) };

	Position northOfRoom{ roomToAddPos.xPos, roomToAddPos.yPos + 1 };
	Position southOfRoom{ roomToAddPos.xPos, roomToAddPos.yPos - 1 };
	Position eastOfRoom{ roomToAddPos.xPos + 1, roomToAddPos.yPos };
	Position westOfRoom{ roomToAddPos.xPos - 1, roomToAddPos.yPos };

	auto roomIter{ std::find_if(m_rooms.begin(), m_rooms.end(), [northOfRoom](auto pair) { return (northOfRoom.xPos == pair.position.xPos) && (northOfRoom.yPos == pair.position.yPos); }) };
	if (roomIter != m_rooms.end()) neighbors.insert({'N', roomIter->id});

	roomIter = std::find_if(m_rooms.begin(), m_rooms.end(), [southOfRoom](auto pair) { return (southOfRoom.xPos == pair.position.xPos) && (southOfRoom.yPos == pair.position.yPos); });
	if (roomIter != m_rooms.end()) neighbors.insert({ 'S', roomIter->id });


	roomIter = std::find_if(m_rooms.begin(), m_rooms.end(), [eastOfRoom](auto pair) { return (eastOfRoom.xPos == pair.position.xPos) && (eastOfRoom.yPos == pair.position.yPos); });
	if (roomIter != m_rooms.end()) neighbors.insert({ 'E', roomIter->id });

	roomIter = std::find_if(m_rooms.begin(), m_rooms.end(), [westOfRoom](auto pair) { return (westOfRoom.xPos == pair.position.xPos) && (westOfRoom.yPos == pair.position.yPos); });
	if (roomIter != m_rooms.end()) neighbors.insert({ 'W', roomIter->id });

	return neighbors;
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

void Dungeon::printMap()
{
	std::cout << "Printing map\n";

	for (const auto &entry : m_rooms)
	{
		std::cout << "Room ID: " << entry.id << '\n';
		std::cout << "Room Type: " << Enums::printRoomType(entry.type) << '\n';
		std::cout << "Adjacent Rooms: ";

		for (const auto &room : entry.adjRooms)
		{
			std::cout << room.second << room.first << ' ';
		}

		std::cout << '\n';
	}
}