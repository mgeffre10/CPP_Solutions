#pragma once
#ifndef TEXTADVENTURE_PLAYER
#define TEXTADVENTURE_PLAYER

#include "Structs.h"

#include <map>
#include <string>
#include <string_view>

class Player
{
private:

	int m_health;
	int m_maxHealth;
	int m_minDamageRange;
	int m_maxDamageRange;
	std::map<std::string, int> m_inventory;
	int m_previousRoom;
	int m_currentRoom;

public:

	Player(int health = 100, int minDamageRange = 30, int maxDamageRange = 45)
		: m_health{ health },
		m_maxHealth{ health },
		m_minDamageRange{ minDamageRange },
		m_maxDamageRange{ maxDamageRange },
		m_inventory{},
		m_previousRoom{ 0 },
		m_currentRoom{ 1 }
	{}

	bool isFullHealth() const { return m_health == m_maxHealth; }
	int getHealth() const { return m_health; }
	int getMaxHealth() const { return m_maxHealth; }
	void restoreHealth() { m_health = m_maxHealth; }
	void heal(int value);

	void takeDamage(int value);
	int calculateDamage();
	
	void goBackToStart() { m_currentRoom = 1; }
	int getCurrentRoom() const { return m_currentRoom; }
	void move(char direction, int destinationId);
	void moveBack();
	
	std::map<std::string, int>::iterator findItem(std::string_view key) { return m_inventory.find(std::string(key)); }
	int getItemCount(std::string_view key) { return findItem(key) == m_inventory.end() ? 0 : findItem(key)->second; }
	void addItem(std::string_view key);
	void removeItem(std::string_view key);

	std::map<std::string, int>& getInventory() { return m_inventory; }
	void clearInventory() { m_inventory.clear(); }
	void printInventory();
	void printStatus();

};

#endif