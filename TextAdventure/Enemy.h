#pragma once
#ifndef TEXTADVENTURE_ENEMY
#define TEXTADVENTURE_ENEMY

class Enemy
{
private:
	int m_health{};
	int m_maxHealth{};
	int m_minDamageRange{};
	int m_maxDamageRange{};

protected:
	void setHealth(int health);
	void setMaxHealth(int health);
	void setMinDamageRange(int minDamageRange);
	void setMaxDamageRange(int maxDamageRange);
public:

	int getHealth();
	int calculateDamage();
	void takeDamage(int value);
	virtual void printEntryDescription();

};

#endif