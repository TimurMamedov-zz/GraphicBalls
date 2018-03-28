#include "graphicscene.h"
#include "graphicball.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTime>
#include <thread>
#include <functional>

GraphicScene::GraphicScene(QObject *parent)
    :QGraphicsScene(parent),
      forces(balls, mut, cond_var, finish),
      calculateForcesThread(std::ref(GraphicScene::forces))
{
    setSceneRect(0, 0, 1000, 800);
    qsrand(QTime::currentTime().msec());
    auto N = qrand() % 10;
    for(int i = 0, j = N; i < N; i++, j--)
    {
        auto ball = new GraphicBall(this);
        ball->setX(qrand() % 500);
        ball->setY(qrand() % 300);
        addItem(ball);
        balls[ball] = ball->pos();
    }

    calculateForcesThread.detach();
    startTimer(1000/50);
}

GraphicScene::~GraphicScene()
{
    finish = true;
    cond_var.notify_one();
    calculateForcesThread.join();
}
/*!
The method that is called by the ball, if it moves
*/
void GraphicScene::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000/200);
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

            startTimer(1000/50);
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

/*!
The timer method, which is called periodically and
which calls the isMovingItems () method
*/
void GraphicScene::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    auto itemsMoved = isMovingItems();

    if (!itemsMoved)
    {
        killTimer(timerId);
        timerId = 0;
    }
}

/*!
The method that assigns coordinates to the balls,
which are calculated in a separate thread
and returns false if the balls stop moving
\param[out] Stop or keep moving
*/
bool GraphicScene::isMovingItems()
{
    std::lock_guard<std::mutex> lk(mut);
    bool isMovingFlag = true;
    for(auto& ball : balls)
    {
        if(movingItem == ball.first)
            continue;
        if(ball.first->pos() == ball.second)
            isMovingFlag = false;
        else
        {
            ball.first->setPos(ball.second);
            isMovingFlag = true;
        }
    }
    cond_var.notify_one();;
    return isMovingFlag;
}
