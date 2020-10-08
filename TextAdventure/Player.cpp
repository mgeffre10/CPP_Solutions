#include "Player.h"
#include "Structs.h"

#include <iostream>
#include <map>

int Player::getCurrentRoom() { return m_currentRoom; }
int Player::getHealth() { return m_health; }
int Player::getMaxHealth() { return m_maxHealth; }
bool Player::isFullHealth() { return getHealth() == getMaxHealth(); }

void Player::heal(int value)
{
	int newHealth{ m_health + value };

	newHealth > getMaxHealth() ? m_health = m_maxHealth : m_health = newHealth;
}

void Player::takeDamage(int value)
{
	int newHealth{ m_health - value };

	newHealth < 0 ? m_health = 0 : m_health = newHealth;
}

void Player::move(char direction, int destinationId)
{
	m_previousRoom = m_currentRoom;
	m_currentRoom = destinationId;
}

void Player::moveBack()
{
	m_currentRoom = m_previousRoom;
	m_previousRoom = 0;
}

int Player::calculateDamage()
{
	std::cout << "Player calculate Damage and Crit chance\n";
	return 10;
}

void Player::addItem(std::string_view key)
{
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

std::map<std::string, int>::iterator Player::findItem(std::string_view key) { return m_inventory.find(std::string(key)); }
int Player::getItemCount(std::string_view key) { return findItem(key) == m_inventory.end() ? 0 : findItem(key)->second; }

void Player::removeItem(std::string_view key)
{
	std::cout << "Removing item: " << key << '\n';
}
void Player::printInventory()
{
	std::cout << "Your inventory: \n";
	for (const auto item : m_inventory)
	{
		std::cout << item.first << ": " << item.second << '\n';
	}
}

void Player::printStatus()
{
	std::cout << "Your health: " << getHealth() << " / " << getMaxHealth() << '\n';
	printInventory();
}