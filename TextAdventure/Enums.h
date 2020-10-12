#pragma once
#ifndef TEXTADVENTURE_ENUMS
#define TEXTADVENTURE_ENUMS

#include <string_view>

enum class Size
{
	Small,
	Large,

	Size_Count
};

enum class RoomType
{
	Start,
	Exit,
	Enemy,
	Potion,
	GemShard,
	Empty,

	RoomType_Count
};

enum class CombatStatus
{
	PlayerDied,
	EnemyDied,
	PlayerFled,

	CombatStatus_Count
};

enum class GameStatus
{
	PlayerDied,
	PlayerExited,

	GameStatus_Count
};

namespace Enums
{
	static inline std::string_view printSize(Size size)
	{
		switch (size)
		{
			case Size::Large: return "Large";
			case Size::Small: return "Small";
			default: return "Unknown";
		}
	}

	static inline std::string_view printRoomType(RoomType type)
	{
		switch (type)
		{
			case RoomType::Empty: return "Empty";
			case RoomType::Enemy: return "Enemy";
			case RoomType::Exit: return "Exit";
			case RoomType::GemShard: return "Gem Shard";
			case RoomType::Potion: return "Potion";
			case RoomType::Start: return "Start";
			default: return "Invalid";
		}
	}
}

#endif