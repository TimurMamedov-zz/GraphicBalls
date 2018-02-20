#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicScene(QObject *parent = Q_NULLPTR);

    void itemMoved();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    int timerId = 0;
};

#endif // GRAPHICSCENE_H
