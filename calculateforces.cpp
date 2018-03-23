#include "calculateforces.h"
#include "graphicscene.h"
#include <QDebug>
#include <QGraphicsItem>

CalculateForces::CalculateForces(std::unordered_map<QGraphicsItem*, QPointF>& balls_,
                                 std::mutex& mut_,
                                 std::condition_variable& cond_var_)
    :balls(balls_), mut(mut_), cond_var(cond_var_)
{

}

void CalculateForces::operator()()
{
    while (true)
    {
        // Sum up all forces pulling items together
        qreal xvel = 0;
        qreal yvel = 0;
        std::unique_lock<std::mutex> lk(mut);

        for(auto ball: balls)
        {
            for(auto anotherBall : balls)
            {
                if (ball.first == anotherBall.first)
                    continue;

                auto vec = ball.first->mapToItem(anotherBall.first, 0, 0);
                auto dx = vec.x();
                auto dy = vec.y();

                if ( qAbs(ball.first->x()) - qAbs(anotherBall.first->x()) < 0.5 )
                    xvel = 0.0001;
                else if(dx)
                {
                    if(dx < 0)
                        xvel += ( (1/dx) - (1/(dx*dx)) );
                    else
                        xvel -= ( (1/dx) - (1/(dx*dx)) );
                }

                if ( qAbs(ball.first->y()) - qAbs(anotherBall.first->y()) < 0.5 )
                    yvel = 0.0001;
                else if(dy)
                {
                    if(dy < 0)
                        yvel += ( (1/dy) - (1/(dy*dy)) );
                    else
                        yvel -= ( (1/dy) - (1/(dy*dy)) );
                }
            }
            balls[ball.first] = ball.first->pos() + QPointF(xvel, yvel);
        }
        cond_var.wait(lk);
    }
}
