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

#include <QtWidgets>

#include "diagramitem.h"
#include "arrow.h"
#include "connector.h"

DiagramItem::DiagramItem(DiagramType diagramType, QMenu * contextMenu,
                         QString name, QDomNode &node, int diagId, int inputs,
                         QGraphicsItem* parent, QGraphicsScene * scene)
    : QGraphicsItem(parent, scene),itemName(name),itemNode(node),diagId(diagId)

{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    myPolygon = QPolygonF(QRectF(0,25,150,50));

    taskId = itemNode.attributes().namedItem("id").nodeValue().toInt();
    addChilds(inputs);
}

void DiagramItem::addChilds(int inputs)
{
    QRectF rec = boundingRect();
    qreal middle = rec.width()/2;

    qreal sep = 10;
    qreal size = 10;

    int id =0;

    if(inputs%2 == 1)
    {
        qreal begin = middle -((qreal)((float)inputs/2)* size + (inputs/2)* sep);

        for(int i=0;i<inputs;i++)
        {
            QRectF rect(begin +i*(sep+size),10,10,10);
            this->childs.append(new Connector(rect,id,this,scene()));
            id++;
        }
    }
    else
    {
        qreal begin = middle -((inputs/2)* size +(qreal)((float)inputs/2 -0.5)* sep);

        for(int i=0;i<inputs;i++)
        {
            QRectF rect(begin +i*(sep+size),10,10,10);
            this->childs.append(new Connector(rect,id,this,scene()));
            id++;
        }
    }
    /**
     * Output connector, only one
     */
    qreal begin = middle - (0.5*size);
    QRectF rect(begin,80,10,10);
    this->childs.append(new Connector(rect,id,this,scene()));
}
QRectF DiagramItem::boundingRect() const
{
    return QRectF(0,0,150,100);
}

void DiagramItem::updatePoz()
{
    QPointF poz = scenePos();

    itemNode.attributes().namedItem("x").setNodeValue(
                                        QString::number((int)poz.x()));
    itemNode.attributes().namedItem("y").setNodeValue(
                QString::number((int)poz.y()));
}

void DiagramItem::updateId(int newId)
{
    diagId = newId;

    itemNode.attributes().namedItem("diagId").setNodeValue(QString::number(newId));

    /**
     *  Convention: only last connector is output connector
     *  so all inputs are destination or "parentOutId"(arrow destination)
     *  with (type = 1)
     */
    for(int i = 0;i<childs.count()-1;i++)
    {
        childs.at(i)->setArrowParent(newId,1);
    }
    /**
     * output is "parentInId"(arrow start point) attribute and have (type = 0)
     */
    childs.last()->setArrowParent(newId,0);
}

void DiagramItem::removeDomElement()
{
    QDomNode parent = itemNode.parentNode();

    parent.removeChild(itemNode);
}

bool DiagramItem::matchIdAndType(int id, int type)
{
    return (taskId == id) && (DiagramItem::DiagramType(type) == myDiagramType);
}

 void DiagramItem::removeArrows()
 {
     for(int i=0;i<childs.size();i++)
     {
         childs.at(i)->removeArrows();
     }
 }
 void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
 {
     Q_UNUSED(option);
     Q_UNUSED(widget);
     QRectF rec1(0,25,150,50);
     QBrush brush;
     QPen pen;
     brush.setStyle(Qt::SolidPattern);

     if(myDiagramType == DiagramItem::Task)
     {
         brush.setColor(QColor("#9c050a"));
         pen.setColor(Qt::gray);
     }
     else
     {
         brush.setColor(Qt::black);

         pen.setColor(Qt::darkGreen);
     }

     painter->setRenderHint(QPainter::Antialiasing);
     painter->setBrush(brush);
     painter->setPen(pen);
     painter->drawRoundedRect(rec1,10.0,5.0);

     QFont font;
     font.setBold(true);
     font.setPointSize(13);
     painter->setFont(font);

     pen.setColor(Qt::white);
     painter->setPen(pen);
     painter->drawText(rec1,Qt::AlignCenter,QString::number(diagId)
                       + ":" + itemName);
 }

 void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
 {
     scene()->clearSelection();
     setSelected(true);
     myContextMenu->exec(event->screenPos());
 }

