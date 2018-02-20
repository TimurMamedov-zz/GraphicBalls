#include "graphicball.h"

#include "graphicscene.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

GraphicBall::GraphicBall(GraphicScene *graphScene)
    :scene_(graphScene)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void GraphicBall::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // Sum up all forces pulling items together
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (auto item, scene()->items())
    {
        auto ball = qgraphicsitem_cast<GraphicBall *>(item);
        if (!ball || (ball == this))
            continue;

        auto vec = mapToItem(ball, 0, 0);
        auto dx = vec.x();
        auto dy = vec.y();
        if ( qAbs(x()) - qAbs(ball->x()) < 20 )
            xvel = 0;
        else if(dx)
        {
            xvel -= ( (1/dx) - (1/(dx*dx)) );
        }

        if ( qAbs(y()) - qAbs(ball->y()) < 20 )
            yvel = 0;
        else if(dy)
        {
            yvel -= (1/dy - 1/(dy*dy));
        }
        if ( qAbs(x()) - qAbs(ball->x()) < 20 )
            xvel = 0;
    }

    if ((qAbs(xvel) < 0.01 && qAbs(yvel) < 0.01))
        xvel = yvel = 0;


//! [5]

    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(newPos.x());
    newPos.setY(newPos.y());
}

bool GraphicBall::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF GraphicBall::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath GraphicBall::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void GraphicBall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken)
    {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    }
    else
    {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant GraphicBall::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemPositionHasChanged:
        scene_->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void GraphicBall::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphicBall::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
