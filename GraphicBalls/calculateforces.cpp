#include "calculateforces.h"
#include "graphicscene.h"
#include <QDebug>
#include <QGraphicsItem>
#include <cmath>

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

                auto R = std::sqrt(dx*dx + dy*dy); ///< force between balls

                if(R > ballPair.first->boundingRect().width()/10) ///< to avoid a force close to infinity when the balls approach each other close
                {
                    vel = (1/R) - (1/(R*R));
                    xvel += vel * dx/R; ///< R*cos(angle)
                    yvel += vel * dy/R; ///< R*sin(angle)
                }
            }
            balls[ballPair.first] = ballPair.first->pos() + QPointF(xvel, yvel); ///< new position
        }
        if(finish)
            break;
        cond_var.wait(lk); ///< wait for the signal from the main thread to calculate the new positions of the balls
    }
}
