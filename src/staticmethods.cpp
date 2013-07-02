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

#include "staticmethods.h"
#include <QMessageBox>
#include <QDebug>
#include <QDomDocument>
#include <QCheckBox>
#include <QDir>

#include "model/projecttreemodel.h"
#include "diagram/diagramwindow.h"

void StaticMethods::deleteItem(Ide *parent, QModelIndexList indexes)
{
    /**
     * Not the best solution, layout can't be changed
     * To be replaced with a custom Dialog.
     */
    QMessageBox delMsg;

    delMsg.setText("Do you really want to delete selected files?");

    QCheckBox deleteFile("Delete File from filesystem", &delMsg);

    deleteFile.blockSignals(true);

    delMsg.addButton(&deleteFile, QMessageBox::ActionRole);

    QAbstractButton *pYES = (QAbstractButton*)delMsg.addButton(QMessageBox::Yes);
    QAbstractButton *pNO  = (QAbstractButton*)delMsg.addButton(QMessageBox::No);
    Q_UNUSED(pYES); // it's used, but to supress warnings //
    delMsg.exec();

    if(delMsg.clickedButton() == pNO)
        return;
    Q_FOREACH(QModelIndex index, indexes)
    {

        QDomNode toDelete = parent->model->getItem(index)->getNode();

        if(deleteFile.checkState() == Qt::Checked)
        {
            /**
             * Get file path and remove file if checkbox is checked.
             */
            if(toDelete.nodeName() == "file")
            {
                QString filePath(index.data(ProjectTreeModel::FileNameRole).toString());
                if(!QFile::remove(filePath))
                    qDebug() << "StaticMethods::DeleteItem: Error! Unable to delete item";
            }
            if(toDelete.nodeName() == "task")
            {
                QString path(parent->whcFile);
                path = path.remove(path.split("/").last());
                path += "src/"
                        + toDelete.attributes().namedItem("name").nodeValue();
                QDir dir (path);
                removeDirectory(dir);
            }
            if(toDelete.nodeName() == "group")
            {
                QString path(parent->whcFile);
                path = path.remove(path.split("/").last());
                path += "data/"
                        + toDelete.attributes().namedItem("name").nodeValue();
                QDir dir(path);
                removeDirectory(dir);
            }

        }

        /**
         * Delete all diagram items associated with deleted task/data
         */
        if(toDelete.nodeName() == "task")
        {
            int itemId = toDelete.attributes().namedItem("id").nodeValue().toInt();
            parent->getDiagram()->deleteItemById(itemId, 0);
        }

        if(toDelete.nodeName() == "group")
        {
            int itemId = toDelete.attributes().namedItem("id").nodeValue().toInt();
            parent->getDiagram()->deleteItemById(itemId, 1);
        }

        QDomNode parents = toDelete.parentNode();

        parents.removeChild(toDelete);

        /** Remove item from project tree **/
        parent->model->deleteItem(index);
    }
}

bool StaticMethods::removeDirectory(QDir &aDir)
{
    bool has_err = false;
    if(aDir.exists())//QDir::NoDotAndDotDot
    {
        QFileInfoList entries = aDir.entryInfoList(QDir::NoDotAndDotDot |
        QDir::Dirs | QDir::Files);
        int count = entries.size();
        for(int idx = 0; idx < count; idx++)
        {
            QFileInfo entryInfo = entries[idx];
            QString path = entryInfo.absoluteFilePath();
            if(entryInfo.isDir())
            {
                QDir next(path);
                has_err = removeDirectory(next);
            }
            else
            {
                QFile file(path);
                if (!file.remove())
                has_err = true;
            }
        }
        if(!aDir.rmdir(aDir.absolutePath()))
        has_err = true;
    }

    return(has_err);
}
