#pragma once
#ifndef TEXTADVENTURE_DUNGEON
#define TEXTADVENTURE_DUNGEON

#include "Structs.h"

#include <map>
#include <vector>
class Dungeon
{
private:

	std::map<Room, std::map<char, int>> m_rooms{};

public:

	Dungeon() {}
	Dungeon(int numOfRooms)
	{
		generateRoomTypes(numOfRooms);
		generateAdjacentRooms();
	}

	void generateRoomTypes(int numOfRooms);
	void generateAdjacentRooms();
	std::map<char, int> getAdjacentRooms(int roomId);
	std::vector<char> getOpenDirections(std::map<char, int> currentRoom, std::map<char, int> adjRoom);
	std::pair<Room, std::map<char, int>> getRoomById(int roomId);
	void printMap();
};

#endif