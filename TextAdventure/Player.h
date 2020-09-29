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
	std::map<std::string_view, int> m_inventory;
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

	bool isFullHealth();
	void heal(int value);
	void takeDamage(int value);
	void move(char direction);
	void moveBack();
	int calculateDamage();
	int getCurrentRoom();
	int getHealth();
	int getMaxHealth();
	void pickUpItem(RoomType roomType);
	void addItem(std::string_view key);
	int findItem(std::string_view key);
	void removeItem(std::string_view key);
	void printInventory();
	void printStatus();

};

#endif