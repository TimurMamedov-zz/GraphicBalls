#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <mutex>
#include <unordered_map>
#include "calculateforces.h"

class GraphicBall;

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicScene(QObject *parent = Q_NULLPTR);
    void itemMoved(GraphicBall *ball);

    std::unordered_map<GraphicBall*, QPointF> points;
    std::mutex mut;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    int timerId = 0;
    CalculateForces forces;
};

#endif // GRAPHICSCENE_H
