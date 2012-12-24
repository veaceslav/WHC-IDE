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

#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QDomDocument>

#include "connector.h"

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class QDomNode;
class ExecNode;

class Arrow : public QGraphicsLineItem, public QObject
{
public:
    enum { Type = UserType + 4 };

    Arrow(Connector *startItem, Connector *endItem, QDomNode nod,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    /**
     * @brief type - return item type, used to distinguish arrows, diagramitems
     *               and connectors
     */
    int type() const
        { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    /**
     * @brief startItem - start Item's center postion is arrow source
     */
    Connector *startItem() const
        { return myStartItem; }
    /**
     * @brief endItem - end Item's center is where arrow points
     */
    Connector *endItem() const
        { return myEndItem; }

    /**
     * @brief updatePosition - update to new position after an item was moved
     */
    void updatePosition();

    /**
     * @brief removeDomNode - remove xml Node when arrow is deleted
     */
    void removeDomNode();

    /**
     * @brief setParentId - when DiagamItem's id was changed, all arrows that
     *                      points to it's connectors must update info from
     *                      arrow tag, or it won't find the correct node when
     *                      project is reloaded.
     * @param id          - new diagram item id
     * @param type        - set it as "parentInId" or as "parentOutId"
     */
    void setParentId(int id, int type);

    /**
     * @brief getExecData - get Execution data - get all info like
     *                     startItem's/endItem's DiagramItem task name, diagId
     *                     ,type,max number of Connectors and connector to which
     *                      points the arrow
     * @return            - a pair of ExecNode, ExecNode - container to store
     *                      info about task/data
     */
    QPair<ExecNode,ExecNode> getExecData();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    Connector *myStartItem;
    Connector *myEndItem;
    QDomNode arrowNod;
    QColor myColor;
    QPolygonF arrowHead;
};

#endif // ARROW_H
