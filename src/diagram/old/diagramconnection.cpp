#include "diagramconnection.h"

#include <QPainter>
#include <QPolygonF>
#include <QDebug>

#include <cmath>

static const double Pi = 3.14159265358979323846264338327950288419717;

DiagramConnection::DiagramConnection(DiagramItem *from, DiagramItem *to,
         QGraphicsItem *parent)
    : QGraphicsItem(parent), source(from), dest(to), arrowSize(10)
{
    setAcceptedMouseButtons(0);

    // update the diagramConnections
    source->diagramConnections.append(this);
    dest->diagramConnections.append(this);
    source->children.append(dest);
    dest->parents.append(source);

    updatePosition();
}

DiagramConnection::~DiagramConnection()
{
    int index;

    index = source->diagramConnections.indexOf(this);
    if (index != -1)
        source->diagramConnections.removeAt(index);
    else
        qDebug()<<"Cannot remove source diagramConnection from task ID" << source->id;

    index = dest->diagramConnections.indexOf(this);
    if (index != -1)
        dest->diagramConnections.removeAt(index);
    else
        qDebug()<<"Cannot remove dest diagramConnection from task ID" << source->id;
}

QRectF DiagramConnection::boundingRect() const
{
    if (!source || !dest)
        return QRectF();
    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                 destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void DiagramConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
          QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);

    if(qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrow

    qreal angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2 * Pi - angle;

    QPointF arrowP1 = destPoint - QPointF(sin(angle + Pi / 3) * arrowSize,
                                            cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = destPoint - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                          cos(angle + Pi - Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << destPoint << arrowP1 << arrowP2);
}

void DiagramConnection::updatePosition()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    //qreal length = line.length();

    prepareGeometryChange();
    // TODO sourcePoint and destPoint need to be calculated to make this work as intended
    sourcePoint = line.p1();
    destPoint = line.p2();
}
