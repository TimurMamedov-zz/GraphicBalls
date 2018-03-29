#include "calculateforcestest.h"

CalculateForcesTest::CalculateForcesTest():culcF(balls, mut, cond_var, finish), point1(10, 10), point2(15, 15),
calcFThread(std::ref(culcF)), predStart([this]()
{
    this->setPos();
    return false;
}), predStop([this]()
{
    while(std::fabs(balls[ball2].x() - balls[ball1].x()) > ball1->boundingRect().width()/10)
    {
        this->setPos();
    }
    return false;
})
{
}

void CalculateForcesTest::SetUp()
{
    std::lock_guard<std::mutex> lk(mut);
    ball1 = new GraphicBall;
    ball1->setPos(point1);
    balls[ball1] = ball1->pos();

    ball2 = new GraphicBall;
    ball2->setPos(point2);
    balls[ball2] = ball2->pos();
}

void CalculateForcesTest::TearDown()
{
    finish = true;
    cond_var.notify_one();
    calcFThread.join();

    for(auto ball : balls)
    {
        delete ball.first;
    }
}

void CalculateForcesTest::setPos()
{
    cond_var.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::lock_guard<std::mutex> lk(mut);
    ball1->setPos(balls[ball1]);
    ball2->setPos(balls[ball2]);
}

QPointF CalculateForcesTest::calculateNewPosition(GraphicBall *ball)
{
    auto vel = qreal{0};
    auto xvel = qreal{0};
    auto yvel = qreal{0};

    auto ballOldPos = ball->pos();

    for(const auto& anotherBallPair: balls)
    {
        if(ball != anotherBallPair.first)
        {
            auto anotherBallOldPos = anotherBallPair.first->pos();

            auto dx = anotherBallOldPos.x() - ballOldPos.x();
            auto dy = anotherBallOldPos.y() - ballOldPos.y();

            auto R = std::sqrt(dx*dx + dy*dy);

            vel = (1/R) - (1/(R*R));
            xvel += vel * dx/R;
            yvel += vel * dy/R;
        }
    }

    return ballOldPos + QPointF(xvel, yvel);
}
