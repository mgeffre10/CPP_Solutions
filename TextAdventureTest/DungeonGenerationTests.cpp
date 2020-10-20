#include "pch.h"
//#include "../TextAdventure/Enums.h"
#include "DungeonTest.h"
#include "../TextAdventure/Spider.cpp"

TEST(DungeonGenerationTest, ValidateNumberOfRoomsGenerated)
{
	int roomCount{ 20 };
	DungeonTest dungeon{ };
	dungeon.generateRooms(roomCount);

	EXPECT_EQ(static_cast<int>(dungeon.getRooms().size()), roomCount);
}
TEST(DungeonGenerationTest, ValidateRoomTypeCount)
{
	int roomCount{ 20 };
	DungeonTest dungeon{ };
	dungeon.generateRooms(roomCount);

	int expectedGemRooms{ 4 };
	int expectedEnemyRooms{ static_cast<int>(std::floor(roomCount * .4)) };
	int expectedPotionRooms{ static_cast<int>(std::ceil(roomCount * .4)) - 4 };

	std::vector<Room> tempRooms{ dungeon.getRooms() };

	int actualGemRooms{ static_cast<int>(std::count_if(
				tempRooms.begin(),
				tempRooms.end(),
				[](const Room& room) { return room.type == RoomType::GemShard; }))
	};

	int actualEnemyRooms{ static_cast<int>(std::count_if(
				tempRooms.begin(),
				tempRooms.end(),
				[](const Room& room) { return room.type == RoomType::Enemy; }))
	};

	int actualPotionRooms{ static_cast<int>(std::count_if(
				tempRooms.begin(),
				tempRooms.end(),
				[](const Room& room) { return room.type == RoomType::Potion; }))
	};

	EXPECT_EQ(expectedGemRooms, actualGemRooms);
	EXPECT_EQ(expectedEnemyRooms, actualEnemyRooms);
	EXPECT_EQ(expectedPotionRooms, actualPotionRooms);
}

TEST(DungeonGenerationTest, ValidateMapSizeEqualsRoomCount)
{
	int roomCount{ 20 };
	DungeonTest dungeon{};
	dungeon.generateRooms(roomCount);
	dungeon.generateMap();

	EXPECT_EQ(static_cast<int>(dungeon.getDungeonMap().size()), roomCount);
}

TEST(DungeonGenerationTest, ValidateRoomTypesAreSeparate)
{
	int roomCount{ 20 };
	DungeonTest dungeon{};
	dungeon.generateRooms(roomCount);
	dungeon.generateMap();

	std::vector<Room> dungeonMap{ dungeon.getDungeonMap() };

	for (const Room& room : dungeonMap)
	{
		int roomTypesSame{ 0 };
		if (room.type != RoomType::Empty)
		{
			roomTypesSame = std::count_if(
						dungeonMap.begin(),
						dungeonMap.end(),
						[dungeonMap](const Room& entry) {
							for (auto connectedRoom : entry.connectedRooms)
							{
								auto roomIter{ std::find_if(dungeonMap.begin(), dungeonMap.end(), [connectedRoom](const Room& room) {return room.id == connectedRoom.second; }) };
								return entry.type == roomIter->type;
							}
						});

			EXPECT_EQ(roomTypesSame, 0);
		}
	}
}

TEST(DungeonGenerationTest, ValidateStartAndExitAreSeparate)
{
	int roomCount{ 20 };
	DungeonTest dungeon{};
	dungeon.generateRooms(roomCount);
	dungeon.generateMap();

	std::vector<Room> dungeonMap{ dungeon.getDungeonMap() };


	ASSERT_NE(static_cast<int>(dungeonMap.size()), 0) << "Dungeon map is empty.";

	bool isStartConnectedToExit{ false };
	auto startRoom{ std::find_if(dungeonMap.begin(), dungeonMap.end(), [](const Room& room) {return room.type == RoomType::Start; }) };
	auto exitRoom{ std::find_if(dungeonMap.begin(), dungeonMap.end(), [](const Room& room) {return room.type == RoomType::Exit; }) };

	for (const auto &connectedRoom : startRoom->connectedRooms)
	{
		if (connectedRoom.second == exitRoom->id) isStartConnectedToExit = true;
	}

	EXPECT_FALSE(isStartConnectedToExit);
}