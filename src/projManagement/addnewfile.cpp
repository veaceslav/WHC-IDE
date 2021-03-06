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

#include <QApplication>
#include <QLineEdit>
#include <QDomDocument>

#include "staticmethods.h"
#include "model/projecttreemodel.h"
#include "projManagement/addnewfile.h"
#include "projManagement/overwritefile.h"


AddNewFile::AddNewFile(QDomDocument *proj, Ide *parent, QModelIndex selected)
    :QDialog(parent)
{
    Q_UNUSED(proj);

    projectXml = proj;
    this->parent = parent;

    tasksItem = parent->model->getItem(parent->model->tasksIndex());
    this->resize(390, 148);
    this->setWindowTitle("Add New File");

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    label = new QLabel(this);
    label->setText("File Name:");

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(QRect(100, 20, 241, 23));

    label_2 = new QLabel(this);
    label_2->setText("Choose Task:");

    comboBox = new QComboBox(this);

    gridLayout.addWidget(label, 0, 0);
    gridLayout.addWidget(lineEdit, 0, 1);
    gridLayout.addWidget(label_2, 1, 0);
    gridLayout.addWidget(comboBox, 1, 1);
    orizontalLayout.addWidget(buttonBox);
    verticalLayout.addLayout(&gridLayout);
    verticalLayout.addLayout(&orizontalLayout);
    verticalLayout.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setLayout(&verticalLayout);

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

    overwriteDialog = NULL;
}

AddNewFile::~AddNewFile()
{
    StaticMethods::destroyObj(&buttonBox);
    StaticMethods::destroyObj(&label);
    StaticMethods::destroyObj(&lineEdit);
    StaticMethods::destroyObj(&label_2);
    StaticMethods::destroyObj(&comboBox);
}

void AddNewFile::slotAddNewFile()
{
    QString path = parent->whcFile;

    path.remove(path.split("/").last());

    path.append("src/" + comboBox->itemText(comboBox->currentIndex()) + "/");
    QString fileName = lineEdit->text();
    path.append(fileName);

    QFile file(path);
    if(!file.open(QFile::ReadOnly))
    {
        slotWriteNewFile();
    }
    else
    {
        file.close();
        overwriteDialog = new OverwriteFile(this, fileName);
        QObject::connect(overwriteDialog, SIGNAL(signalOvewriteAccepted()),
                         this, SLOT(slotWriteNewFile()));
        overwriteDialog->show();
    }
}

void AddNewFile::slotWriteNewFile()
{
    /**
     *  No need to search node by name,
     *  we put them in order in combobox
     *  so the index is just enough
     */
    QDomNode task = lst.at(comboBox->currentIndex());
    ProjectTreeItem *taskItem = tasksItem->child(comboBox->currentIndex());

    QDomElement elem = projectXml->createElement("file");
    elem.setAttribute("name", lineEdit->text());
    task.appendChild(elem);

    parent->model->addItem(new ProjectTreeItem(elem, taskItem), taskItem);


    QString path = parent->whcFile;

    path.remove(path.split("/").last());

    path.append("src/" +
                comboBox->itemText(comboBox->currentIndex()) +
                + "/" + lineEdit->text());

    QFile file(path);
    /**
      * Opening a file in write mode
      * will create a new file if it doesn't exist
      */
    file.open(QFile::WriteOnly);
    file.close();
    this->close();
}

