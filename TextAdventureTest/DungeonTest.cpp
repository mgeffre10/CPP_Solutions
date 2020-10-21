//#include "pch.h"
#include "DungeonTest.h"
#include "../TextAdventure/Dungeon.h"

void DungeonTest::generateRooms(int roomCount)
{
	Dungeon dungeon{};
	m_rooms = dungeon.generateRoomTypes(roomCount);
}

void DungeonTest::generateMap()
{
	Dungeon dungeon{ 20 };
	m_dungenMap = dungeon.getRooms();
}
