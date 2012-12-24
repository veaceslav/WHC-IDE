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

#include "projManagement/datagroup.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QDomDocument>
#include <QDir>
#include "model/projecttreeitem.h"
#include "diagram/diagramwindow.h"


DataGroup::DataGroup(QDomDocument* proj, Ide *parent)
    : QDialog(parent)
{

    projectXml = proj;

    this->parent = parent;

    this->resize(391, 80);
    this->setWindowTitle(QString("Add New Data Group"));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setGeometry(QRect(30, 40, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    taskLabel = new QLabel(this);
    taskLabel->setGeometry(QRect(15, 10, 79, 21));
    taskLabel->setText("Group Name:");

    taskName = new QLineEdit(this);
    taskName->setObjectName(QString::fromUtf8("lineEdit"));
    taskName->setGeometry(QRect(100, 10, 271, 22));


    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotDataGroup()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

DataGroup::~DataGroup()
{
}

void DataGroup::slotDataGroup()
{

    if(taskName->text().isEmpty())
    {
        QMessageBox msg;
        msg.setText("No Data Group name specified.");
        msg.setInformativeText("Please enter group name.");
        msg.exec();
        return;
    }
    QDomNode nod =projectXml->lastChild();

    QDomNode groups = nod.namedItem("data");

    QDomElement tmp = projectXml->createElement("group");


    tmp.setAttribute(QString("name"),taskName->text());
    int id = groups.lastChild().attributes().namedItem("id").nodeValue().toInt()
             + 1;
    tmp.setAttribute("id",id);

    QDomText txt = projectXml->createTextNode("");

    tmp.appendChild(txt);
    groups.appendChild(tmp);


    QModelIndex taskIndex = parent->model->dataIndex();

    ProjectTreeItem* parentItem = static_cast<ProjectTreeItem*>(taskIndex.internalPointer());

    parent->model->addItem(new ProjectTreeItem(tmp,parentItem),parentItem);


    /**
     *  Create task directory in src folder
     */
    QString temp(parent->whcFile);

    temp.remove(temp.split("/").last());
    temp.append("data/" + taskName->text());

    QDir dir;
    dir.mkdir(temp);

    /**
     *  Add new group to diagram data comboBox
     */
    parent->getDiagram()->addData(taskName->text(),id);

    this->close();
}
