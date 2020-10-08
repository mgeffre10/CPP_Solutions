#include "Player.h"
#include "Structs.h"

#include <iostream>
#include <map>

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

void Player::move(char direction, int destinationId)
{
	m_previousRoom = m_currentRoom;
	m_currentRoom = destinationId;
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

void Player::addItem(std::string_view key)
{
	std::cout << "Adding item: " << key << '\n';

	auto item{ findItem(key) };
	if (item != m_inventory.end())
	{
		++(item->second);
	}
	else
	{
		m_inventory.insert({ std::string(key), 1 });
	}
}

std::map<std::string, int>::iterator Player::findItem(std::string_view key)
{
	std::cout << "Finding item: " << key << '\n';
	return m_inventory.find(std::string(key));
}

int Player::getItemCount(std::string_view key)
{
	if (findItem(key) == m_inventory.end()) return 0;
	return findItem(key)->second;
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