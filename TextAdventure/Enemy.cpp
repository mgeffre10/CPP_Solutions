#include "Enemy.h"

#include <iostream>
void Enemy::setHealth(int health)
{
	m_health = health;
}

void Enemy::setMaxHealth(int health)
{
	m_maxHealth = health;
}

void Enemy::setMinDamageRange(int minDamageRange)
{
	m_minDamageRange = minDamageRange;
}

void Enemy::setMaxDamageRange(int maxDamageRange)
{
	m_maxDamageRange = maxDamageRange;
}

int Enemy::getHealth()
{
	return m_health;
}
int Enemy::calculateDamage()
{
	std::cout << "Enemy calculate Damage and Crit chance\n";
	return 10;
}

void Enemy::takeDamage(int value)
{
	std::cout << "Enemy taking damage of " << value << '\n';
}

void Enemy::printEntryDescription()
{
	std::cout << "Enemy description\n";
}