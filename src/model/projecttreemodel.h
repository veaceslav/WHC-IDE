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

#ifndef PROJECTTREEMODEL_H
#define PROJECTTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDomDocument>
#include <QFileInfo>
#include "projecttreeitem.h"

class ProjectTreeItem;
class Ide;

class ProjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ProjectTreeModel(QString filename, Ide* parent = 0);
    ~ProjectTreeModel();

    /** Methods that must be implemented for QAbstractItemModel interface **/
    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    QModelIndex index (int row, int column, const QModelIndex &parent) const;

    QModelIndex parent (const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief setData - set data, usually display name, to an item
     * @param index   - Item's index
     * @param value   - Value, now only a QString
     * @param role    - Only EditRole is used for now
     * @return        - true is no error occurred
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /** Used by project Management : add file / delete file **/

    ProjectTreeItem* getItem(QModelIndex index);

    void addItem(ProjectTreeItem* elem, ProjectTreeItem *parentItem);

    void deleteItem(QModelIndex index);

    ProjectTreeItem* getGroupByName(QString name);

    /**
      * @brief getProjectXml - gets the project xml used by the current project
      * @return              - returns a reference to the QDomDocument
      *                        containing the project xml
      */
    QDomDocument* getProjectXml();

    QFileInfo file() const;

    /** Used in DiagramScene **/

    QModelIndex tasksIndex() const;
    QModelIndex dataIndex() const;

    enum {
        IdRole = Qt::UserRole + 1,
        FileNameRole,
        PositionRole
    };


signals:

    /**
     * @brief updateDiagram - it's role is to tell the diagram window to
     *                        rename all the items that have the old name
     *                        with the new one
     * @param oldName - the name used before the rename
     * @param newName - the name used to replace the old name
     */
    void updateDiagram(QString oldName, QString newName);


private:

    void setupModelData(QDomNode nodx, ProjectTreeItem *parent);

    ProjectTreeItem* rootItem;
    QDomDocument project;
    QFileInfo projectInfo;
    Ide* parentIde;
    friend class TestDiagramScene;
};
#endif // PROJECTTREEMODEL_H
