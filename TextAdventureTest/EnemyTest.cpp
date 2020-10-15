#include "pch.h"
#include "Enums.h"
#include "Enemy.h"
#include "Spider.h"

namespace EnemyCreation
{
	TEST(Create_Enemy, Small_Spider)
	{
		Spider spider{ Size::Small };

		EXPECT_EQ(spider.getSize(), Size::Small);
	}

	TEST(Create_Enemy, Large_Spider)
	{
		Spider spider{ Size::Large };

		EXPECT_EQ(spider.getSize(), Size::Large);
	}
}