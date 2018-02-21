#include "graphicscene.h"
#include "graphicball.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTime>
#include <thread>
#include <functional>

GraphicScene::GraphicScene(QObject *parent)
    :QGraphicsScene(parent), forces(*this)
{
    setSceneRect(0, 0, 1000, 800);
    qsrand(QTime::currentTime().msec());
    auto N = qrand() % 20;
    for(int i = 0, j = N; i < N; i++, j--)
    {
        auto ball = new GraphicBall(this);
        ball->setX(qrand() % 500);
        ball->setY(qrand() % 300);
        addItem(ball);
    }
    std::thread secondThread(std::ref(GraphicScene::forces));
    secondThread.detach();
}

void GraphicScene::itemMoved(GraphicBall *ball)
{
//    std::lock_guard<std::mutex> lk(mut);
//    if(points.find(ball) != points.end())
//        ball->setPos(points[ball]);
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
