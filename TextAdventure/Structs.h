#pragma once
#ifndef TEXTADVENTURE_STRUCTS
#define TEXTADVENTURE_STRUCTS

#include "Enums.h"
#include "Enemy.h"

struct Room
{
	int id{};
	RoomType type{ RoomType::Empty };
	bool bHasBeenVisited{ false };
	int itemCount{ 0 };
	Enemy enemy{};

	Room()
	{
		static int roomId{ 1 };
		id = roomId++;
	}

	Room(RoomType roomType) : Room()
	{
		type = roomType;
	}

	bool operator==(const Room &room) {
		return id == room.id;
	}

	bool operator<(const Room &room) const {
		return id < room.id;
	}
};

#endif