#include "graphicscene.h"
#include "graphicball.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTime>
#include <thread>
#include <functional>

GraphicScene::GraphicScene(QObject *parent)
    :QGraphicsScene(parent), forces(balls, mut, cond_var)
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
        balls[ball] = ball->pos();
    }
    std::thread secondThread(std::ref(GraphicScene::forces));
    secondThread.detach();
}

void GraphicScene::itemMoved(GraphicBall *ball)
{
    if (!timerId)
        timerId = startTimer(1000/100);
}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    std::lock_guard<std::mutex> lk(mut);
    if(mouseEvent->buttons() & Qt::RightButton)
    {
        auto item = itemAt(mouseEvent->scenePos(), QTransform());
        if(item)
        {
            balls.erase(item);
            removeItem(item);
        }
        else
        {
            auto ball = new GraphicBall(this);
            ball->setPos(mouseEvent->scenePos());
            addItem(ball);
            balls.emplace(ball, ball->pos());
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    std::lock_guard<std::mutex> lk(mut);
    movingItem = Q_NULLPTR;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    std::lock_guard<std::mutex> lk(mut);
    auto ball = itemAt(mouseEvent->scenePos(), QTransform());
    if(ball)
    {
        movingItem = ball;
        auto ballPair = balls.find(ball);
        if(ballPair != balls.end())
        {
            ballPair->second = ballPair->first->pos();
        }
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicScene::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    auto itemsMoved = isMovingItems();

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

bool GraphicScene::isMovingItems()
{
    std::lock_guard<std::mutex> lk(mut);
    for(auto ball : balls)
    {
        if(movingItem == ball.first)
            continue;
        ball.first->setPos(ball.second);
    }
    cond_var.notify_one();;
    return true;
}
