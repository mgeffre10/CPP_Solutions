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
		generateConnections();
	}

	std::vector<Room> generateRoomTypes(int numOfRooms);
	void generateAdjacentRooms(std::vector<Room> &rooms);
	void generateConnections();
	int getAdjRoomCount(const Room &room);
	char getPotentialDirection(const Room &currentRoom);
	bool isValidPosition(const Room &room, const std::map<char, int> &neighbors);
	void propogateNewRoom(int newRoomId, const std::map<char, int> neighbors);
	std::vector<char> getOpenDirections(const std::map<char, int> &room);
	std::vector<Room>::iterator getRoomById(int roomId);
	std::map<char, int> getNeighbors(Position currentRoomPos, char direction);
	Position determinePosition(Position currentRoomPos, char direction);
	void printMap();
};

#endif