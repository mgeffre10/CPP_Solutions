#pragma once

#include "pch.h"

#include "../TextAdventure/Structs.h"
#include "../TextAdventure/Spider.h"
class DungeonTest
{
private:
	std::vector<Room> m_rooms;
	std::vector<Room> m_dungenMap;

public:

	std::vector<Room> getRooms() { return m_rooms; }
	std::vector<Room> getDungeonMap() { return m_dungenMap; }

	void generateRooms(int roomCount);
	void generateMap();
};