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

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QString>
#include <QDomDocument>

class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
class Connector;

class Arrow;

class DiagramItem : public QGraphicsItem
{
public:
    enum {Type = UserType + 15};

    enum DiagramType {Task,Data};

    DiagramItem(DiagramType DiagramType, QMenu *contextMenu,
                QString name, QDomNode& node, int diagId, int inputs =1,
                QGraphicsItem * parent = 0, QGraphicsScene *scene =0
                );

    /**
     * @brief removeArrows - diagram item has no arrows, but this children does
     *                       so before deleting a diagram item, we delete all
     *                       children's arrows;
     */
    void removeArrows();

    QRectF boundingRect() const;

    /**
     * @brief diagramType -Task or Data
     */
    DiagramType diagramType() const
                { return myDiagramType; }

    QPolygonF polygon() const
                { return myPolygon; }

    QString Name() const
                 { return itemName; }
                 
    void setName(QString newName)
                 { itemName = newName; }

    int connectorCount()
                 { return childs.count(); }
    /**
     * @brief type        is used to identify it as DiagramItem,
     *                    so no arrows will be set between DiagramItems,
     *                    only Connectors
     */
    int type() const
            { return Type; }
    /**
     * @brief id - return diagram id, it's NOT the task id from .whc file
     * @return - diagram id
     */
    int id() const
            { return diagId;}

    Connector* childAt(int index)
                      { return childs.at(index); }

    /**
     * @brief updatePoz - if item was moved, this method is called to update
     *                    it's position in .whc file
     */
    void updatePoz();

    /**
     * @brief updateId - update diagram id of diagram item. Since diagram can
     *                   have many changes like deleting items that will fragment
     *                   indexes, all items id are rewritten after load.
     * @param newId    - new diagram item's id
     */
    void updateId(int newId);

    /**
     * @brief removeDomElement - when diagram item is deleted from scene,
     *                           delete it's tag from .whc project file
     */
    void removeDomElement();

    bool matchIdAndType(int id, int type);

protected:

    /**
     * @brief paint - reimplemented protected function, using painter we draw
     *                a custom shape for this item, in our case this is a
     *                rectangle and task name
     */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:

    void addChilds(int inputs);
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QMenu * myContextMenu;
    QList<Connector*> childs;
    QString itemName;
    QDomNode itemNode;
    int taskId;
    int diagId;



};

#endif // DIAGRAMITEM_H
