#ifndef GRAPHICBALL_H
#define GRAPHICBALL_H

class GraphicScene;

#include <QGraphicsItem>

class GraphicBall : public QGraphicsItem
{
public:
    GraphicBall() = default;
    GraphicBall(GraphicScene *graphScene);
    enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    GraphicScene *scene_ = Q_NULLPTR;
};

#endif // GRAPHICBALL_H
