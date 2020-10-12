#pragma once
#ifndef TEXTADVENTURE_SPIDER
#define TEXTADVENTURE_SPIDER

#include "Enemy.h"
#include "Enums.h"

class Spider : public Enemy
{
private:

	Size m_size{};

public:

	Spider() = delete;

	Spider(Size size) : Enemy()
	{
		m_size = size;

		if (size == Size::Small)
		{
			setHealth(60);
			setMaxHealth(60);
			setMinDamageRange(10);
			setMaxDamageRange(15);
		}
		else if (size == Size::Large)
		{
			setHealth(120);
			setMaxHealth(120);
			setMinDamageRange(20);
			setMaxDamageRange(30);
		}
	}

	void printEntryDescription() override;
	Size getSize() { return m_size; }
};

#endif