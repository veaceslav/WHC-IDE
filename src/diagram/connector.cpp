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

#include "connector.h"
#include "containers.h"
#include "arrow.h"
#include "diagramscene.h"
#include <QGraphicsScene>
#include <QDebug>

Connector::Connector(QRectF bound,int id, QGraphicsItem* parent,
                     QGraphicsScene* scene)
    : QGraphicsItem(parent), connectorId(id)
{
    pressed = false;
    bounds = bound;
    myPolygon = QPolygonF(boundingRect());
    //TODO documentat mai bine si asta
    if(scene)
        scene->addItem(this);
}

QRectF Connector::boundingRect() const
{
    return bounds;
}

void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();

    QBrush brush(Qt::blue);

    if(pressed)
    {
        brush.setColor(Qt::red);
    }
    else
    {
        brush.setColor(Qt::darkGreen);
    }


    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(brush);
    painter->drawEllipse(rec);

}
void Connector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    pressed = true;
    update();
}

void Connector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    pressed = false;
    update();
}

void Connector::removeArrow(Arrow *arrow)
 {
     int index = arrows.indexOf(arrow);

     if (index != -1)
         arrows.removeAt(index);
 }

 void Connector::removeArrows()
 {
     foreach (Arrow *arrow, arrows)
     {
         arrow->startItem()->removeArrow(arrow);
         arrow->endItem()->removeArrow(arrow);
         arrow->removeDomNode();
         scene()->removeItem(arrow);
         ((DiagramScene*)scene())->deleteArrow(arrow);
         delete arrow;
     }
 }

 void Connector::addArrow(Arrow *arrow)
 {
     arrows.append(arrow);
 }

QPointF Connector::centerPoz()
{
    return bounds.center();
}

void Connector::setArrowParent(int id, int type)
{
    for(int i = 0; i < arrows.count(); i++)
    {
        arrows.at(i)->setParentId(id, type);
    }
}
