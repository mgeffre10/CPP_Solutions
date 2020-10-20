#include "pch.h"
#include "../TextAdventure/Player.h"
#include "../TextAdventure/Player.cpp"
TEST(InventoryTest, AddItemToInventory)
{
	// Arrange
	Player player{};

	// Act
	player.addItem("Potion");

	// Assert
	EXPECT_EQ(player.getInventory().find("Potion"), player.getInventory().begin());
}

TEST(InventoryTest, AddTwoOfSameItemToInventory)
{
	// Arrange
	Player player{};

	// Act
	player.addItem("Potion");
	player.addItem("Potion");

	// Assert
	EXPECT_EQ(static_cast<int>(player.getInventory().size()), 1);
	EXPECT_EQ(player.getInventory().find("Potion"), player.getInventory().begin());
	EXPECT_EQ(player.getInventory().find("Potion")->second, 2);
}

TEST(InventoryTest, ItemFoundInInventory)
{
	// Arrange
	Player player{};
	player.addItem("Potion");

	// Act & Assert
	EXPECT_NE(player.findItem("Potion"), player.getInventory().end());
}

TEST(InventoryTest, ItemNotFoundInInventory)
{
	// Arrange
	Player player{};
	player.addItem("Potion");

	// Act & Assert
	EXPECT_EQ(player.findItem("Gem Shard"), player.getInventory().end());
}

TEST(InventoryTest, DecrementInventoryItemByOne) {
	// Arrange
	Player player{};
	player.addItem("Potion");
	player.addItem("Potion");

	// Act
	player.removeItem("Potion");

	// Assert
	EXPECT_NE(player.getInventory().find("Potion"), player.getInventory().end());
	EXPECT_EQ(player.getInventory().at("Potion"), 1);
}

TEST(InventoryTest, DecrementInventoryByOneAndRemoveFromInventory)
{
	// Arrange
	Player player{};
	player.addItem("Potion");

	// Act
	player.removeItem("Potion");

	// Assert
	EXPECT_EQ(player.getInventory().find("Potion"), player.getInventory().end());
}

TEST(InventoryTest, ClearInventory)
{
	// Arrange
	Player player{};

	player.addItem("Potion");
	player.addItem("Potion");
	player.addItem("Gem Shard");

	// Act
	player.clearInventory();

	// Assert
	EXPECT_EQ(static_cast<int>(player.getInventory().size()), 0);
}