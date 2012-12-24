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

#include "projManagement/addnewfile.h"
#include <QApplication>
#include "model/projecttreemodel.h"
#include <QLineEdit>
#include <QDomDocument>

AddNewFile::AddNewFile(QDomDocument* proj, Ide* parent, QModelIndex selected)
    :QDialog(parent)
{
    Q_UNUSED(proj);

    projectXml = proj;
    this->parent = parent;

    tasksItem = parent->model->getItem(parent->model->tasksIndex());
    this->resize(390, 148);
    this->setWindowTitle("Add New File");

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setGeometry(QRect(40, 110, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    label = new QLabel(this);
    label->setGeometry(QRect(10, 20, 91, 21));
    label->setText("File Name:");

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(QRect(100, 20, 241, 23));

    label_2 = new QLabel(this);
    label_2->setGeometry(QRect(10, 60, 91, 16));
    label_2->setText("Choose Task:");

    comboBox = new QComboBox(this);
    comboBox->setGeometry(QRect(100, 60, 241, 24));

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAddNewFile()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


    QDomNode nod =proj->lastChild();

    QDomNode tasks = nod.namedItem("tasks");

    lst = tasks.childNodes();

    for(int i=0;i<lst.count();i++)
        comboBox->addItem(lst.at(i).attributes().namedItem("name").nodeValue());

    /** set default task that was selected when context menu is opened **/
    if(selected.isValid())
        comboBox->setCurrentIndex(selected.row());
}

AddNewFile::~AddNewFile()
{
}

void AddNewFile::slotAddNewFile()
{
    /**
     *  No need to search node by name,
     *  we put them in order in combobox
     *  so the index is just enough
     */
    QDomNode task = lst.at(comboBox->currentIndex());
    ProjectTreeItem* taskItem = tasksItem->child(comboBox->currentIndex());

    QDomElement elem = projectXml->createElement("file");
    elem.setAttribute("name",lineEdit->text());
    task.appendChild(elem);

    parent->model->addItem(new ProjectTreeItem(elem,taskItem),taskItem);

    QString path = parent->whcFile;

    path.remove(path.split("/").last());

    path.append("src/" +
                comboBox->itemText(comboBox->currentIndex())+
                + "/" + lineEdit->text());

    /**
      * Opening a file in write mode
      * will create a new file if it doesn't exist
      */
    QFile file(path);
    file.open(QFile::WriteOnly);
    file.close();

    this->close();
}

