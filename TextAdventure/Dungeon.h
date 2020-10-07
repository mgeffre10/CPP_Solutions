#pragma once
#ifndef TEXTADVENTURE_DUNGEON
#define TEXTADVENTURE_DUNGEON

#include "Structs.h"

#include <map>
#include <vector>
class Dungeon
{
private:

	std::vector<Room> m_rooms{};

public:

	Dungeon() {}
	Dungeon(int numOfRooms)
	{
		std::vector<Room> tempRoomArray{ generateRoomTypes(numOfRooms) };
		generateAdjacentRooms(tempRoomArray);
	}

	std::vector<Room> generateRoomTypes(int numOfRooms);
	void generateAdjacentRooms(std::vector<Room> &rooms);
	int getConnectionCount(const Room &room);
	char getPotentialDirection(const Room &currentRoom);
	bool isValidPosition(const Room &currentRoom, const Room &adjRoom);
	std::vector<char> getOpenDirections(const std::map<char, int> &room);
	std::vector<Room>::iterator roomAtPosition(Position position);
	std::vector<Room>::iterator getRoomById(int roomId);
	char getOppositeDirection(char direction);
	Position determinePosition(Position currentRoomPos, char direction);
	void printMap();
};

#endif