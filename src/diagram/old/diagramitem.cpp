#include "diagramitem.h"

#include <QFontMetricsF>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "diagramscene.h"

DiagramItem::DiagramItem(const QModelIndex &index, QGraphicsItem *parent)
                 : QGraphicsItem(parent),padding(5),persistentIndex(index)
{
    int taskId = index.data(ProjectTreeModel::IdRole).toInt();
    QString taskName = index.data(Qt::DisplayRole).toString();
    QPointF taskPos = index.data(ProjectTreeModel::PositionRole).toPointF();

    this->id = taskId;
    this->name = taskName;
    this->setPos(taskPos);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(ItemSendsGeometryChanges);

    setZValue(-1);
}

DiagramItem::~DiagramItem()
{
    foreach (DiagramConnection *diagramConnection, diagramConnections) {
        scene()->removeItem(diagramConnection);
        delete diagramConnection;
    }
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        if (scene())
            scene()->model()->setData(index(), value.toPointF(), ProjectTreeModel::PositionRole);

        foreach (DiagramConnection *diagramConnection, diagramConnections)
            diagramConnection->updatePosition();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF DiagramItem::boundingRect() const
{
    QFontMetricsF fm(font);
    return fm.boundingRect(label).adjusted(-padding - 1, -padding - 1, padding + 1, padding + 1);
}

QPainterPath DiagramItem::shape() const
{
    QPainterPath path;
    QFontMetrics fm(font);
    path.addRoundedRect(fm.boundingRect(label).adjusted(-padding, -padding, padding, padding),
                        50, 75, Qt::RelativeSize);
    return path;
}

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::white);
    painter->setFont(font);

    QFontMetricsF fm(font);

    painter->save();
    if (option->state & QStyle::State_Selected) // Highlight the task if it is selected
        painter->setBrush(Qt::gray);
    painter->drawRoundedRect(fm.boundingRect(label).adjusted(-padding, -padding, padding, padding),
                             25, 25, Qt::RelativeSize);
    painter->restore();

    painter->drawText(fm.boundingRect(label), Qt::AlignCenter, label);

    painter->restore();
}

DiagramScene *DiagramItem::scene() const
{
    return qobject_cast<DiagramScene*>(QGraphicsItem::scene());
}
