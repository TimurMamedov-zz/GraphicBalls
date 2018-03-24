#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <mutex>
#include <unordered_map>
#include <condition_variable>
#include <thread>
#include "calculateforces.h"

class GraphicBall;

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicScene(QObject *parent = Q_NULLPTR);
    void itemMoved();
    ~GraphicScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    CalculateForces forces;
    std::thread calculateForcesThread;
    int timerId = 0;
    bool isMovingItems();
    const QGraphicsItem *movingItem = Q_NULLPTR;

    std::unordered_map<QGraphicsItem*, QPointF> balls;
    std::mutex mut;
    std::condition_variable cond_var;
    bool finish = false;
};

#endif // GRAPHICSCENE_H
