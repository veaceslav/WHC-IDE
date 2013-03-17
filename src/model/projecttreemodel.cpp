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

#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "projecttreeitem.h"
#include "projecttreemodel.h"
#include "ide.h"

ProjectTreeModel::ProjectTreeModel(QString filename,
                                   QDomDocument*& projectXml, Ide *parent)
    : QAbstractItemModel(parent),projectInfo(filename),parentIde(parent)
{

    project =  QDomDocument("WHC");
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!project.setContent(&file)){
        qDebug() << "Error!";
        file.close();
        return;
    }
    file.close();

    rootItem = new ProjectTreeItem(project, 0);

    setupModelData(project, rootItem);

    projectXml = &project;

}

ProjectTreeModel::~ProjectTreeModel()
{
    delete rootItem;
}

int ProjectTreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return static_cast<ProjectTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();

}

bool ProjectTreeModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    ProjectTreeItem* item = getItem(index);
    QDomNode itemNode = item->getNode();

    QString type = itemNode.nodeName();

    if(type != "file" && type != "task" && type != "group")
            return false;

    QString oldName, newName;

    if(type == "file")
    {
        oldName = data(index, ProjectTreeModel::FileNameRole).toString();
<<<<<<< HEAD
        //Building the new name for the file requires us to truncate
        //the old name and then append the new name
=======
        /*Building the new name for the file requires us to truncate*/
        /*the old name and then append the new name*/
>>>>>>> 7833da41f99fedf4ec1d725fbde09f7aab0b4504
        newName = oldName;
        newName.truncate(newName.lastIndexOf(item->getName()));
        newName += value.toString();

        if(!QFile::rename(oldName, newName))
            return false;
        if(!item->setData(value, role))
        {
            if(!QFile::rename(oldName, newName))
                qDebug() << "Could not revert "<< newName
                         <<" to " << oldName << "after rename fail";
            return false;
        }

    }
<<<<<<< HEAD
    else    //type == "task" or type == "group"
=======
    else    /*type == "task" or type == "group"*/
>>>>>>> 7833da41f99fedf4ec1d725fbde09f7aab0b4504
    {
        QString oldFile = itemNode.attributes().namedItem("name").nodeValue();
        QString newFile = value.toString();
        QString folder;

        if(type == "task")
            folder = "/src/";
<<<<<<< HEAD
        else    //type == "group"
=======
        else    /*type == "group"*/
>>>>>>> 7833da41f99fedf4ec1d725fbde09f7aab0b4504
            folder = "/data/";

        oldName = projectInfo.path() + folder + oldFile;
        newName = projectInfo.path() + folder + newFile;

        QDir dir;
        if(!dir.rename(oldName, newName))
            return false;

        if(!item->setData(value, role))
        {
            if(!dir.rename(newName, oldName))
                qDebug() << "Could not revert " << newName
                         << " to " << oldName << "after rename fail";
            return false;
        }

        emit updateDiagram(oldFile, newFile);
    }

<<<<<<< HEAD
=======
    /*Everything went well, so we write the changes to the xml file*/
>>>>>>> 7833da41f99fedf4ec1d725fbde09f7aab0b4504
    parentIde->writeXmltoFile();

    return true;
}

QVariant ProjectTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

        ProjectTreeItem* item = static_cast<ProjectTreeItem*>(index.internalPointer());

        if (role == FileNameRole)
        {
            QString fileName(item->data(role).toString());
            if (fileName.isEmpty())
                return QString();
            else
                return projectInfo.path() + fileName;
        }
        return item->data(role);

}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return  rootItem->data(section);

    return QVariant();
}

QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column,parent))
        return QModelIndex();

    ProjectTreeItem* parentItem;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    ProjectTreeItem* childItem = parentItem->child(row);

    if(childItem)
        return createIndex(row,column,childItem);
    else
    {
        qDebug() << "Projecttreemode::index: Warning! No child item found";
        return QModelIndex();
    }
}

QModelIndex ProjectTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        qDebug() << "Projecttreemode::parent: Warning index is not valid";
        return QModelIndex();
    }

    ProjectTreeItem * childItem = static_cast<ProjectTreeItem*>(index.internalPointer());

    ProjectTreeItem* parentItem = childItem->parent();

    if(parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(),0,parentItem);
}

int ProjectTreeModel::rowCount(const QModelIndex &parent) const
{
    ProjectTreeItem* parentItem;

    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    return parentItem->childCount();

}
void ProjectTreeModel::setupModelData(QDomNode nodx, ProjectTreeItem *parent)
{
    if(!nodx.hasChildNodes())
        return;

    QDomNodeList lst = nodx.childNodes();

    for(int i =0;i<lst.size();i++)
    {
        if(lst.at(i).nodeName() == "diagram")
            return;
        ProjectTreeItem* tmp = new ProjectTreeItem(lst.at(i),parent);
        parent->appendChild(tmp);

        if(lst.at(i).hasChildNodes())
            setupModelData(lst.at(i),tmp);
    }
}

ProjectTreeItem* ProjectTreeModel::getItem(QModelIndex index)
{
    ProjectTreeItem* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    return item;
}

void ProjectTreeModel::addItem(ProjectTreeItem *elem, ProjectTreeItem* parentItem)
{
    emit layoutAboutToBeChanged();

    parentItem->appendChild(elem);

    /** Write changes to .whc file **/
    parentIde->writeXmltoFile();

    emit layoutChanged();
}

void ProjectTreeModel::deleteItem(QModelIndex index)
{
    emit layoutAboutToBeChanged();

    QModelIndex parent = index.parent();
    ProjectTreeItem* parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    parentItem->deleteChild(index.row());

    /** Write changes to .whc file **/
    parentIde->writeXmltoFile();

    emit layoutChanged();
}

ProjectTreeItem* ProjectTreeModel::getGroupByName(QString name)
{
    QModelIndex index = dataIndex();

    ProjectTreeItem* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    for(int i=0;i<item->childCount();i++)
    {
        ProjectTreeItem* child = item->child(i);
        if(child->getName() == name)
            return child;
    }
    return 0;
}
QModelIndex ProjectTreeModel::tasksIndex() const
{
    QModelIndex root = index(0, 0, QModelIndex());
    return index(0, 0, root);
}

QModelIndex ProjectTreeModel::dataIndex() const
{
    QModelIndex root = index(0, 0,QModelIndex());
    return index(1, 0, root);
}

QFileInfo ProjectTreeModel::file() const
{
    return projectInfo;
}
