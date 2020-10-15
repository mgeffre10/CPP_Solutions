#include "pch.h"

/*TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}*/


TEST(AddFourAndThree, AddNumbers) {
	EXPECT_EQ(4 + 3, 7);
}

TEST(AddFourAndTwo, AddNumbers) {
	EXPECT_EQ(4 + 2, 8);
}