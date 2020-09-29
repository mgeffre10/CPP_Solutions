#include "Player.h"
#include "Structs.h"

#include <iostream>

bool Player::isFullHealth()
{
	std::cout << "isFullHealth()\n";
	return true;
}

void Player::heal(int value)
{
	std::cout << "Heal for " << value << '\n';
}

void Player::takeDamage(int value)
{
	std::cout << "Player take damage for " << value << '\n';
}

void Player::move(char direction)
{
	std::cout << "Move character in the direction of " << direction << '\n';
}

void Player::moveBack()
{
	std::cout << "Move player back.\n";
}

int Player::calculateDamage()
{
	std::cout << "Player calculate Damage and Crit chance\n";
	return 10;
}

int Player::getCurrentRoom()
{
	std::cout << "Get Current Room.\n";
	return m_currentRoom;
}
int Player::getHealth()
{
	return m_health;
}
int Player::getMaxHealth()
{
	return m_maxHealth;
}
void Player::pickUpItem(RoomType roomType)
{
	std::cout << "Picking up item: " << static_cast<int>(roomType) << '\n';
}

void Player::addItem(std::string_view key)
{
	std::cout << "Adding item: " << key << '\n';
}

int Player::findItem(std::string_view key)
{
	std::cout << "Finding item: " << key << '\n';
	return 0;
}

void Player::removeItem(std::string_view key)
{
	std::cout << "Removing item: " << key << '\n';
}
void Player::printInventory()
{
	std::cout << "Printing inventory.\n";
}

void Player::printStatus()
{
	std::cout << "Printing player status\n";
	printInventory();
}