#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QString>
#include <QFont>
#include <QRectF>
#include <QPointF>
#include <QPainterPath>
#include <QVariant>
#include <QPersistentModelIndex>

#include "diagramconnection.h"

class DiagramConnection;
class DiagramScene;

class DiagramItem : public QGraphicsItem
{
public:
    explicit DiagramItem(const QModelIndex &index, QGraphicsItem *parent = 0);
    ~DiagramItem();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    enum { Type = UserType + 1 };
    virtual int type() const { return Type; }
    QModelIndex index() { return persistentIndex; }

    DiagramScene *scene() const;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    int id;                     // unique task id in project
    QString name;               // task name
    QString label;              // task label
    QPointF position;           // graphical position

    int padding;                // padding between the rounded rect and the text (in pixels)
    QFont font;

    QPersistentModelIndex persistentIndex;
private:
    QList<DiagramItem*> parents;    // tasks that are pre to this
    QList<DiagramItem*> children;   // tasks that are post to this

    QList<DiagramConnection*> diagramConnections; // list of connections

    friend class DiagramConnection;
    friend class DiagramScene;
};

#endif // DIAGRAMITEM_H
