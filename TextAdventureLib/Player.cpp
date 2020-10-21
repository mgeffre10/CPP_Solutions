#include "Player.h"
#include "Random.h"
#include "Structs.h"

#include <iostream>
#include <map>

void Player::heal(int value)
{
	int newHealth{ m_health + value };

	newHealth > getMaxHealth() ? m_health = m_maxHealth : m_health = newHealth;
	removeItem("Potion");
}

void Player::takeDamage(int value)
{
	int newHealth{ m_health - value };

	newHealth < 0 ? m_health = 0 : m_health = newHealth;
}

int Player::calculateDamage()
{
	int critChance{ Random::getRandomNumberInRange(0, 100) };
	int damage{ Random::getRandomNumberInRange(m_minDamageRange, m_maxDamageRange) };

	if (critChance > 70)
	{
		std::cout << "Critical hit!\n";
		return damage * 2;
	}

	return damage;
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

void Player::removeItem(std::string_view key)
{
	auto item{ findItem(key) };
	if (item != m_inventory.end()) item->second == 1 ? m_inventory.erase(item->first) : --(item->second);
}

void Player::printInventory()
{
	std::cout << "------------Your inventory--------------\n";
	if (m_inventory.size() == 0) std::cout << "Empty\n";

	for (const auto item : m_inventory)
	{
		std::cout << item.first << ": " << item.second << '\n';
	}
	std::cout << "----------------------------------------\n";
}

void Player::printStatus()
{
	std::cout << "Your health: " << m_health << " / " << m_maxHealth << '\n';
	printInventory();
}