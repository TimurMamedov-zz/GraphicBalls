#include "graphicscene.h"
#include "graphicball.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTime>

GraphicScene::GraphicScene(QObject *parent)
    :QGraphicsScene(parent)
{
    qsrand(QTime::currentTime().msec());
    auto N = qrand() % 20;
    for(int i = 0, j = N; i < N; i++, j--)
    {
        auto ball = new GraphicBall(this);
        ball->setX(qrand() % 200);
        ball->setY(qrand() % 200);
        addItem(ball);
    }
}

void GraphicScene::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
     if(mouseEvent->buttons() & Qt::RightButton)
     {
         auto item = itemAt(mouseEvent->scenePos(), QTransform());
         if(item)
             removeItem(item);
         else
         {
             auto ball = new GraphicBall(this);
             ball->setPos(mouseEvent->scenePos());
             addItem(ball);
         }
     }
     QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicScene::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<GraphicBall *> balls;
    foreach (auto item, items())
    {
        if (auto ball = qgraphicsitem_cast<GraphicBall *>(item))
            balls << ball;
    }

    foreach (auto ball, balls)
        ball->calculateForces();

    bool itemsMoved = false;
    foreach (auto ball, balls)
    {
        if (ball->advance())
            itemsMoved = true;
    }

    if (!itemsMoved)
    {
        killTimer(timerId);
        timerId = 0;
    }
}
