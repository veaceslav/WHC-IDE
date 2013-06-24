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

#include "arrow.h"
#include "containers.h"
#include "diagramitem.h"
#include <math.h>
#include <QDebug>


 const qreal Pi = 3.14;

 Arrow::Arrow(Connector *startItem, Connector *endItem, QDomNode nod,
          QGraphicsItem *parent, QGraphicsScene *scene)
     : QGraphicsLineItem(parent), arrowNod(nod)
 {
     myStartItem = startItem;
     myEndItem = endItem;
     setFlag(QGraphicsItem::ItemIsSelectable, true);
     myColor = Qt::black;
     setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     //TODO documentat mai bine
     if(scene)
        scene->addItem(this);
 }

 QRectF Arrow::boundingRect() const
 {
     qreal extra = (pen().width() + 20) / 2.0;

     return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                       line().p2().y() - line().p1().y()))
         .normalized()
         .adjusted(-extra, -extra, extra, extra);
 }

 QPainterPath Arrow::shape() const
 {
     QPainterPath path = QGraphicsLineItem::shape();
     path.addPolygon(arrowHead);
     return path;
 }

 void Arrow::updatePosition()
 {
     QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
     setLine(line);
 }

 void Arrow::removeDomNode()
 {
     QDomNode parent = arrowNod.parentNode();

     parent.removeChild(arrowNod);
 }

 void Arrow::setParentId(int id, int type)
 {
     if(type == 0)
        arrowNod.attributes().namedItem("parentInId").setNodeValue(QString::number(id));
     else
         arrowNod.attributes().namedItem("parentOutId").setNodeValue(QString::number(id));
 }

QPair<ExecNode, ExecNode> Arrow::getExecData()
 {
     ExecNode source,dest;

     source.diagId = arrowNod.attributes().namedItem("parentInId").nodeValue().toInt();
     source.conId  = arrowNod.attributes().namedItem("connectorInId").nodeValue().toInt();
     source.name = ((DiagramItem*)myStartItem->parentItem())->Name();
     source.maxCon= ((DiagramItem*)myStartItem->parentItem())->connectorCount();
     source.type = ((DiagramItem*)myStartItem->parentItem())->diagramType();

     dest.diagId = arrowNod.attributes().namedItem("parentOutId").nodeValue().toInt();
     dest.conId  = arrowNod.attributes().namedItem("connectorOutId").nodeValue().toInt();
     dest.name = ((DiagramItem*)myEndItem->parentItem())->Name();
     dest.maxCon= ((DiagramItem*)myEndItem->parentItem())->connectorCount();
     dest.type = ((DiagramItem*)myEndItem->parentItem())->diagramType();

     return QPair<ExecNode,ExecNode>(source,dest);
 }

 void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
           QWidget *)
 {
     if(myStartItem->centerPoz().y() <= myEndItem->centerPoz().y())
         return;

     QPen myPen = pen();
     myPen.setColor(myColor);
     qreal arrowSize = 20;
     painter->setPen(myPen);
     painter->setBrush(myColor);

     /** Simple aproach - set a line between connector's center points **/
     QLineF centerLine(mapFromItem(myStartItem,myStartItem->centerPoz()),
                       mapFromItem(myEndItem,myEndItem->centerPoz()));
     setLine(centerLine);

     /** Drawing line + arrow head **/
     double angle = ::acos(line().dx() / line().length());
     if (line().dy() >= 0)
         angle = (Pi * 2) - angle;

         QPointF arrowP1 = line().p2() - QPointF(sin(angle + Pi / 2.5) * arrowSize,
                                         cos(angle + Pi / 2.5) * arrowSize);
         QPointF arrowP2 = line().p2() - QPointF(sin(angle + Pi - Pi / 2.5) * arrowSize,
                                         cos(angle + Pi - Pi / 2.5) * arrowSize);

         arrowHead.clear();
         arrowHead << line().p2() << arrowP1 << arrowP2;
         painter->setRenderHint(QPainter::Antialiasing);
         painter->drawLine(line());
         painter->drawPolygon(arrowHead);

         if (isSelected()) {
             painter->setPen(QPen(myColor, 1, Qt::DashLine));
         QLineF myLine = line();
         myLine.translate(0, 4.0);
         painter->drawLine(myLine);
         myLine.translate(0,-8.0);
         painter->drawLine(myLine);
     }
 }
