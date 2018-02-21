#include "calculateforces.h"
#include "graphicscene.h"
#include "graphicball.h"
#include <QDebug>

CalculateForces::CalculateForces(GraphicScene &graphicScene)
    :scene(graphicScene)
{

}

void CalculateForces::operator()()
{
    while (true)
    {
        // Sum up all forces pulling items together
        qreal xvel = 0;
        qreal yvel = 0;
        scene.mut.lock();
        auto balls = scene.items();
        foreach (auto item, balls)
        {
            auto ball = qgraphicsitem_cast<GraphicBall *>(item);
            if (!ball)
                continue;

            foreach (auto item, scene.items())
            {
                auto anotherBall = qgraphicsitem_cast<GraphicBall *>(item);
                if (!ball || ball == anotherBall)
                    continue;

                auto vec = ball->mapToItem(anotherBall, 0, 0);
                auto dx = vec.x();
                auto dy = vec.y();

                if ( qAbs(ball->x()) - qAbs(anotherBall->x()) < 0.5 )
                    xvel = 0.0001;
                else if(dx)
                {
                    if(dx < 0)
                        xvel += ( (1/dx) - (1/(dx*dx)) );
                    else
                        xvel -= ( (1/dx) - (1/(dx*dx)) );
                }

                if ( qAbs(ball->y()) - qAbs(anotherBall->y()) < 0.5 )
                    yvel = 0.0001;
                else if(dy)
                {
                    if(dy < 0)
                        yvel += ( (1/dy) - (1/(dy*dy)) );
                    else
                        yvel -= ( (1/dy) - (1/(dy*dy)) );
                }
            }
            scene.points[ball] = ball->pos() + QPointF(xvel, yvel);
            scene.mut.unlock();
        }
    }
}
