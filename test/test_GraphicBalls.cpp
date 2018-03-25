#include <gtest/gtest.h>
#include "calculateforces.h"
#include "graphicball.h"
#include <QDebug>
#include <unordered_map>
#include <QGraphicsItem>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <gtest/gtest.h>

class CalculateForcesTest : public ::testing::Test
{
public:
    CalculateForcesTest():culcF(balls, mut, cond_var, finish),
    calcFThread(std::ref(culcF)){}

protected:
    virtual void SetUp()
    {
//        finish = true;
//        cond_var.notify_one();
    }

    virtual void TearDown()
    {
        finish = true;
        cond_var.notify_one();
        calcFThread.join();
    }
    CalculateForces culcF;
    std::thread calcFThread;
    std::unordered_map<QGraphicsItem*, QPointF> balls;
    std::mutex mut;
    std::condition_variable cond_var;
    bool finish;
};

TEST_F(CalculateForcesTest, operator_brackets)
{
    QPointF point1(50, 50);
    QPointF point2(100, 100);

    std::unique_lock<std::mutex> lk(mut);
    auto ball1 = new GraphicBall;
    ball1->setPos(point1);
    balls[ball1] = ball1->pos();

    auto ball2 = new GraphicBall;
    ball2->setPos(point2);
    balls[ball2] = ball2->pos();
    lk.unlock();
    for(auto i = std::size_t{0}; i < 1; i++)
    {
        std::unique_lock<std::mutex> lk(mut);
        ball1->setPos(balls[ball1]);
        ball2->setPos(balls[ball2]);
        cond_var.notify_one();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NE(point1, balls[ball1]);
    EXPECT_NE(point2, balls[ball2]);

//    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
