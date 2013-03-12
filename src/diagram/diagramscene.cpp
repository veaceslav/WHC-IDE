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

#include <QtGui>

#include "diagramscene.h"
#include "diagramwindow.h"
#include "containers.h"
#include "arrow.h"
#include "connector.h"

DiagramScene::DiagramScene(QMenu *itemMenu, QDomDocument*projectXml, DiagramWindow *parent)
    : QGraphicsScene(parent),
      projXml(projectXml)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Task;
    line = 0;
    myLineColor = Qt::black;
    itemCount = 0;
    diagramXml = projXml->elementsByTagName("diagram").at(0);

}
void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

void DiagramScene::renameItems(QString oldName, QString newName) {
    for(int i = 0; i < diagItems.count(); i++)
        if(diagItems.at(i)->Name() == oldName)
            diagItems.at(i)->setName(newName);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    DiagramNode nod;
    QDomNode xmlNode;
    switch (myMode)
    {
        case InsertItem:
            if(((DiagramWindow*)parent())->itemListEmpty(myItemType))
                return;
            nod  = ((DiagramWindow*)parent())->returnCurrentItem(myItemType);
            xmlNode = createItemXml(nod,mouseEvent->scenePos());
            item = new DiagramItem(myItemType, myItemMenu,nod.name,
                                   xmlNode,itemCount,
                                   nod.inputs);
            itemCount++;
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            diagItems.append(item);
            emit itemInserted(item);
            break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
        break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0)
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine)
    {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == Connector::Type &&
                endItems.first()->type() == Connector::Type &&
                startItems.first() != endItems.first())
        {
            Connector *startItem =
                    qgraphicsitem_cast<Connector *>(startItems.first());
            Connector *endItem =
                    qgraphicsitem_cast<Connector *>(endItems.first());
            QDomNode arrowXml =  createArrowXml(startItem,endItem);
            Arrow *arrow = new Arrow(startItem, endItem,arrowXml);

            arrowList.append(arrow);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();

        }
    }

    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    if(myMode == MoveItem)
    {
        DiagramItem* item =(DiagramItem*)itemAt(mouseEvent->scenePos());
        if(item == 0)
            return;
        if(item->type() == DiagramItem::Type)
            {
                item->updatePoz();
                ((DiagramWindow*)parent())->updateXml();
            }
    }
}

QDomNode DiagramScene::createItemXml(DiagramNode nod, QPointF pos)
{
    QDomElement item = projXml->createElement("item");

    item.setAttribute("type",myItemType);
    item.setAttribute("id",nod.id);
    item.setAttribute("diagId",itemCount);
    item.setAttribute("x",pos.x());
    item.setAttribute("y",pos.y());

    diagramXml.appendChild(item);

    ((DiagramWindow*)parent())->updateXml();

    return item;
}

QDomNode DiagramScene::createArrowXml(Connector* start, Connector* end)
{
    QDomElement item = projXml->createElement("arrow");
    DiagramItem* startParent = (DiagramItem*)start->parentItem();
    DiagramItem* endParent   = (DiagramItem*)end->parentItem();

    /**
     * parentInId is arrow source
     */
    item.setAttribute("parentInId",startParent->id());
    item.setAttribute("connectorInId",start->id());

    item.setAttribute("parentOutId",endParent->id());
    item.setAttribute("connectorOutId",end->id());

    diagramXml.appendChild(item);
    ((DiagramWindow*)parent())->updateXml();

    return item;
}

DiagramItem* DiagramScene::getItemById(int id)
{

    for(int i=0;i<diagItems.count();i++)
    {
        DiagramItem* tmp = diagItems.at(i);
        if(tmp->id() == id)
            return tmp;
    }
    return 0; // Null
}

void DiagramScene::loadDiagram()
{
    QDomNodeList lst = projXml->elementsByTagName("item");

    qDebug() << lst.count();

    for(int i=0;i<lst.count();i++)
    {
        QDomNode xmlNode = lst.at(i);
        QDomNamedNodeMap attr = xmlNode.attributes();
        int type = attr.namedItem("type").nodeValue().toInt();
        int id = attr.namedItem("id").nodeValue().toInt();
        int diagId = attr.namedItem("diagId").nodeValue().toInt();

        qreal x = attr.namedItem("x").nodeValue().toDouble();
        qreal y = attr.namedItem("y").nodeValue().toDouble();

        DiagramNode nod = ((DiagramWindow*)parent())->searchById(type,id);

        DiagramItem* item = new DiagramItem(((DiagramItem::DiagramType)type),
                                            myItemMenu,
                                            nod.name,
                                            xmlNode,
                                            diagId,
                                            nod.inputs);

        item->setPos(x,y);

        diagItems.append(item);
        addItem(item);
    }

    QDomNodeList lst2 = projXml->elementsByTagName("arrow");

    for(int i=0;i<lst2.count();i++)
    {
        QDomNamedNodeMap attr = lst2.at(i).attributes();

        DiagramItem* startParent =
                getItemById(attr.namedItem("parentInId").nodeValue().toInt());

        DiagramItem* endParent =
                getItemById(attr.namedItem("parentOutId").nodeValue().toInt());

        Connector* start =
                startParent->childAt(attr.namedItem("connectorInId").nodeValue().toInt());

        Connector* end =
                endParent->childAt(attr.namedItem("connectorOutId").nodeValue().toInt());

        Arrow *arrow = new Arrow(start, end,lst2.at(i));
        arrowList.append(arrow);

        arrow->setColor(myLineColor);
        start->addArrow(arrow);
        end->addArrow(arrow);
        addItem(arrow);
        arrow->setZValue(-1000.0);
        arrow->updatePosition();
    }

    for(int i=0;i<diagItems.count();i++)
    {
        diagItems.at(i)->updateId(itemCount);
        itemCount++;
    }
}

void DiagramScene::deleteArrow(Arrow *arr)
{
    arrowList.removeAt(arrowList.indexOf(arr));
}

void DiagramScene::deleteItemsById(int id, int type)
{

    Q_FOREACH(DiagramItem* item, diagItems)
    {
        if(item->matchIdAndType(id,type))
        {
            item->removeArrows();
            item->removeDomElement();

            this->removeItem(item);

            diagItems.removeAt(diagItems.indexOf(item));
        }
    }
}

void DiagramScene::removeFromList(DiagramItem *item)
{
    diagItems.removeAt(diagItems.indexOf(item));
}

