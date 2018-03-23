#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <mutex>
#include <unordered_map>
#include <condition_variable>
#include "calculateforces.h"

class GraphicBall;

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicScene(QObject *parent = Q_NULLPTR);
    void itemMoved(GraphicBall *ball);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    int timerId = 0;
    CalculateForces forces;
    bool isMovingItems();

    std::unordered_map<QGraphicsItem*, QPointF> balls;
    std::mutex mut;
    std::condition_variable cond_var;
};

#endif // GRAPHICSCENE_H
