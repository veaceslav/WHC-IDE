#ifndef DIAGRAMCONNECTION_H
#define DIAGRAMCONNECTION_H

#include <QGraphicsItem>

#include "diagramitem.h"

class DiagramItem;

class DiagramConnection : public QGraphicsItem
{
public:
    DiagramConnection(DiagramItem *from, DiagramItem *to,
                      QGraphicsItem *parent = 0);
    ~DiagramConnection();

    DiagramItem* from() const { return source; }
    DiagramItem* to() const { return dest; }

    void updatePosition();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

    enum { Type = UserType + 2 };
    int type() const { return Type; }

private:
    DiagramItem *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // DIAGRAMCONNECTION_H
