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

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QDomDocument>
#include "diagramitem.h"


class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QgraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class DiagramWindow;

class DiagramNode;

class DiagramScene : public QGraphicsScene
{

   Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem};

    DiagramScene(QMenu* itemMenu, QDomDocument *projectXml,
                 DiagramWindow *parent = 0);
    QFont font() const
            { return myFont; }
    QColor itemColor() const
            { return myItemColor; }
    QColor lineColor() const
            { return myLineColor; }

    QList<Arrow*> getArrows() const
             { return arrowList; }
    void loadDiagram();

    void deleteArrow(Arrow* arr);

    void deleteItemsById(int id, int type);

    /**
     * @brief removeFromList - remove DiagramItems from class internal list
     * @param item           - item to remove
     */
    void removeFromList(DiagramItem* item);

    /**
     * @brief renameItem - renames all the items in the scene that have the
     *                     old name and replace it with the new name
     *                     (used after a rename in the project tree model)
     * @param oldName - the name of the items that need to be renamed
     * @param newName - the name to replace the old name
     */
    void renameItems(QString oldName, QString newName);


public slots:

    /**
     * @brief setMode set one of two diagram modes:
     * @param mode - lineInsert, ItemInsert
     */
    void setMode(Mode mode);

    /**
     * @brief setItemType - 2 types of Diagam Item: Task and Data
     * @param type
     */
    void setItemType(DiagramItem::DiagramType type);

    /**
     * @brief renameItems - change all the items that have the old name,
     *                      with the new name.
     * @param oldName     - the old name of the item, used to select
     *                       items to be changed
     * @param newName     - the new name that will replace the old name
     */
    void renameItems(QString oldName, QString newName);

signals:

    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

    /**
     * @brief createItemXml - create dom node (xml tag) in .whc file
     * @param nod           - Diagam Node with data about nr of inputs,
     *                        task name etc...
     * @param pos           - save items position to preserve diagram geometry
     * @return              - QDomNode
     */
    QDomNode createItemXml(DiagramNode nod, QPointF pos);

    /**
     * @brief createArrowXml - create dom node (xml tag) for every arrow
     * @param start          - startItem - arrow source
     * @param end            - endItem - arrow destination
     * @return               - arrow QDomNode
     */
    QDomNode createArrowXml(Connector *start, Connector *end);
    /**
     * @brief getItemById - get item by diagram id, because items
     *                      can be deleted, items indexes doesn't match with
     *                      item's id(note: no duplicates)
     * @param id          - DiagramItem's id
     * @return            - DiagramItem with a specific id
     */
    DiagramItem* getItemById(int id);


    DiagramItem::DiagramType myItemType;
    QList<DiagramItem*> diagItems;
    QList<Arrow*> arrowList;
    int itemCount;

    QMenu * myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem * line;
    QDomDocument* projXml;
    QDomNode diagramXml;

    QFont myFont;

    QColor myItemColor;
    QColor myLineColor;
};


#endif // DIAGRAMSCENE_H
