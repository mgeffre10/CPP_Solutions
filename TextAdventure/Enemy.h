#pragma once
#ifndef TEXTADVENTURE_ENEMY
#define TEXTADVENTURE_ENEMY

#include <iostream>

class Enemy
{
private:
	int m_health{};
	int m_maxHealth{};
	int m_minDamageRange{};
	int m_maxDamageRange{};

protected:
	void setHealth(int health) { m_health = health; };
	void setMaxHealth(int health) { m_maxHealth = health; };
	void setMinDamageRange(int minDamageRange) { m_minDamageRange = minDamageRange; };
	void setMaxDamageRange(int maxDamageRange) { m_maxDamageRange = maxDamageRange; };
public:

	int getHealth() { return m_health; };
	int getMaxHealth() { return m_maxHealth; };
	int calculateDamage();
	void takeDamage(int value);
	
	virtual void printEntryDescription() { std::cout << "Enemy description\n"; };
};

#endif