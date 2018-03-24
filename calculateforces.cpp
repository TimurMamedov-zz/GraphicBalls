#include "calculateforces.h"
#include "graphicscene.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QtCore/qmath.h>

CalculateForces::CalculateForces(std::unordered_map<QGraphicsItem*, QPointF>& balls_,
                                 std::mutex& mut_,
                                 std::condition_variable& cond_var_, bool &finish_)
    :balls(balls_), mut(mut_), cond_var(cond_var_), finish(finish_)
{

}

void CalculateForces::operator()()
{
    while (true)
    {
        // Sum up all forces pulling items together
        std::unique_lock<std::mutex> lk(mut);

        for(const auto& ballPair: balls)
        {
            qreal vel = 0;
            qreal xvel = 0;
            qreal yvel = 0;

            for(const auto& anotherBallPair: balls)
            {
                if (ballPair.first == anotherBallPair.first)
                    continue;
                auto vec = anotherBallPair.first->pos();
                auto dx = vec.x() - ballPair.first->x();
                auto dy = vec.y() - ballPair.first->y();

                auto R = qSqrt(dx*dx + dy*dy);

                if(R > ballPair.first->boundingRect().width()/10)
                {
                    vel = (1/R) - (1/(R*R));
                    xvel += vel * dx/R;
                    yvel += vel * dy/R;
                }
            }
            balls[ballPair.first] = ballPair.first->pos() + QPointF(xvel, yvel);
        }
        cond_var.wait(lk);
        if(finish)
            break;
    }
}
