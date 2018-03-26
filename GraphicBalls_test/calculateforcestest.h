#ifndef CALCULATEFORCESTEST_H
#define CALCULATEFORCESTEST_H

#include <gtest/gtest.h>
#include "calculateforces.h"
#include "graphicball.h"
#include <unordered_map>
#include <QGraphicsItem>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <functional>
#include <cmath>

class CalculateForcesTest : public ::testing::Test
{
public:
    CalculateForcesTest();

protected:

    virtual void SetUp();

    virtual void TearDown();

    CalculateForces culcF;
    QPointF point1;
    QPointF point2;
    std::thread calcFThread;
    std::unordered_map<QGraphicsItem*, QPointF> balls;
    std::mutex mut;
    std::condition_variable cond_var;
    bool finish;
    GraphicBall *ball1, *ball2;

    std::function<bool()> predStart;
    std::function<bool()> predStop;

    void usingPredicate(std::function<bool()> pred)
    {
        while(pred()){};
    }

    void setPos();

    QPointF calculateNewPosition(GraphicBall *ball);
};

#endif // CALCULATEFORCESTEST_H
