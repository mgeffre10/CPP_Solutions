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
};

struct Room
{
	int id{};
	RoomType type{ RoomType::Empty };
	bool bHasBeenVisited{ false };
	Enemy enemy{};
	Position position{};
	std::map<char, int> connectedRooms{};

	Room()
	{
		static int roomId{ 1 };
		id = roomId++;
	}

	Room(RoomType roomType) : Room()
	{
		type = roomType;
	}
};

#endif