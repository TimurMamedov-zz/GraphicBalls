#include "calculateforcestest.h"

TEST_F(CalculateForcesTest, start)// check for start
{
    usingPredicate(predStart);

    EXPECT_NE(point1, ball1->pos());
    EXPECT_NE(point2, ball2->pos());
}

TEST_F(CalculateForcesTest, correctPosition)// check for correct Position
{
    auto ball1NewPos = calculateNewPosition(ball1);
    auto ball2NewPos = calculateNewPosition(ball2);

    usingPredicate(predStart);

    EXPECT_DOUBLE_EQ(ball1->x(), ball1NewPos.x());
    EXPECT_DOUBLE_EQ(ball1->y(), ball1NewPos.y());

    EXPECT_DOUBLE_EQ(ball2->x(), ball2NewPos.x());
    EXPECT_DOUBLE_EQ(ball2->y(), ball2NewPos.y());
}

TEST_F(CalculateForcesTest, stop)// check for stop
{
    usingPredicate(predStop);

    EXPECT_NEAR(balls[ball2].x(), balls[ball1].x(), ball1->boundingRect().width()/10);
    EXPECT_NEAR(balls[ball2].y(), balls[ball1].y(), ball1->boundingRect().width()/10);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
