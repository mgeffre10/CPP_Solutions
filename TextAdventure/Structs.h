#pragma once
#ifndef TEXTADVENTURE_STRUCTS
#define TEXTADVENTURE_STRUCTS

#include "Enums.h"
#include "Enemy.h"

#include <map>

struct Position
{
	int xPos{ 0 };
	int yPos{ 0 };

	bool operator==(const Position &pos) const
	{
		return (xPos == pos.xPos && yPos == pos.yPos);
	}
};

struct Room
{
	int id{};
	RoomType type{ RoomType::Empty };
	bool bHasBeenVisited{ false };
	Enemy* enemy{};
	Position position{};
	std::map<char, int> connectedRooms{};

	Room(int roomId) : id{ roomId }
	{ }

	Room(RoomType roomType, int id) : Room(id)
	{
		type = roomType;
	}
};

#endif