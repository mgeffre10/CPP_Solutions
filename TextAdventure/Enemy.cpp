#include "Enemy.h"
#include "Random.h"

#include <iostream>

int Enemy::calculateDamage()
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

void Enemy::takeDamage(int value)
{
	int newHealth{ m_health - value };

	newHealth < 0 ? m_health = 0 : m_health = newHealth;
}