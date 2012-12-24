/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef Connector_H
#define Connector_H
#include <QPainter>
#include <QGraphicsItem>
//#include <QGraphicsPixmapItem>
#include <QList>


class QPixmap;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;

class Arrow;

class Connector : public QGraphicsItem
{
public:

    enum {Type = UserType + 14};

    Connector(QRectF bound,int id,
              QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);


    /**
     * @brief boundingRect -bounding rect is set by parent when item is
     *                      initialized
     * @return QrectF containing item's bounds in parent's coordonates
     */
    QRectF boundingRect() const;

    QPolygonF polygon() const
                { return myPolygon; }

    /**
     * @brief type        is used to identify it as Connector,
     *                    so no arrows will be set between DiagramItems,
     *                    only Connectors
     */
    int  type() const
                { return Type;}
    /**
     * @brief id - give connector's id, every connector have one id, even if its
     *             input or output
     * @return   connector's id
     */
    int id() const
                {return connectorId;}
    /**
     * @brief addArrow - add arrow to list
     * @param arrow    - arrow to add
     */
    void addArrow(Arrow *arrow);
    /**
     * @brief removeArrow - remove arrow from list
     * @param arrow       - arrow to remove
     */
    void removeArrow(Arrow *arrow);
    /**
     * @brief removeArrows -remove all arrows (when deleting item)
     */
    void removeArrows();

    /**
     * @brief centerPoz -item's center position, used to set arrows
     * @return
     */
    QPointF centerPoz();

    /**
     * @brief setArrowParent - when diagam item is updated, all arrow data
     *                         from xml tags must be updated
     * @param id             - new parent id
     * @param type           - tells arrow if its parentInId or parentOutId.
     */
    void setArrowParent(int id,int type);

    bool pressed;
protected:

    /**
     * @brief paint - reimplemented protected function, draw a circle filled
     *                with color.
     */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

    QList<Arrow *> arrows;
    QPolygonF myPolygon;
    int connectorId;
    //DiagramType myDiagramType;
    QRectF bounds;

};

#endif // Connector_H
