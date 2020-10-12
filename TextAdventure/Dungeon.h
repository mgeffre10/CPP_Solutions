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
	void createRooms(int count, RoomType type, int &currentIndex, std::vector<Room> &rooms);
	int getConnectionCount(const Room &room);
	char getPotentialDirection(const Room &currentRoom);
	std::vector<Room>::iterator getRoomById(int roomId);
	std::vector<char> getOpenDirections(const std::map<char, int> &room);
	char getOppositeDirection(char direction) const;
	std::map<char, int> getConnectedRooms(int currentRoomId);

	bool isValidPosition(const Room &currentRoom, const Room &adjRoom);
	Position determinePosition(Position currentRoomPos, char direction);
	std::vector<Room>::iterator roomAtPosition(Position position);

	void printMap();
	void resetDungeon() { m_rooms.clear(); };
};

#endif