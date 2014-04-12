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

#ifndef PROJECTTREEITEM_H
#define PROJECTTREEITEM_H
#include <QList>
#include <QVariant>
#include <QDomDocument>
#include "projecttreemodel.h"

class ProjectTreeItem
{
public:
    ProjectTreeItem(QDomNode nod, ProjectTreeItem *parent =0);
    ~ProjectTreeItem();

    void appendChild(ProjectTreeItem* child);
    void deleteChild(int poz);
    ProjectTreeItem* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int role) const;

    bool setData(const QVariant &value,int role);
    int row() const;
    QList<ProjectTreeItem*> getChildItems();

    QDomNode getNode();

    bool searchChildByName(QString name);

    inline QString getName() {return itemName; }

    ProjectTreeItem* parent();

private:
    QList<ProjectTreeItem*> childItems;
    ProjectTreeItem* parentItem;
    QString itemName;

    QDomNode node;
};
#endif // PROJECTTREEITEM_H
