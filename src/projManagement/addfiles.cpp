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

#include "projManagement/addfiles.h"
#include <QApplication>
#include "model/projecttreeitem.h"
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>

AddFiles::AddFiles(QDomDocument* proj, Ide* parent,QModelIndex selected)
    :QDialog(parent)
{
    Q_UNUSED(proj);

    projectXml = proj;
    this->parent = parent;

    tasksItem = parent->model->getItem(parent->model->tasksIndex());
    this->resize(390, 148);
    this->setWindowTitle("Add Existing Files");

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setGeometry(QRect(40, 110, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    label = new QLabel(this);
    label->setGeometry(QRect(10, 20, 91, 21));
    label->setText("Choose file(s)");

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(QRect(100, 20, 241, 23));

    label_2 = new QLabel(this);
    label_2->setGeometry(QRect(10, 60, 91, 16));
    label_2->setText("Choose Task:");

    comboBox = new QComboBox(this);
    comboBox->setGeometry(QRect(100, 60, 241, 24));

    pushButton = new QPushButton(this);
    pushButton->setGeometry(QRect(350, 20, 31, 24));
    pushButton->setText("...");


    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAddFiles()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));



    QDomNode nod =proj->lastChild();

    QDomNode tasks = nod.namedItem("tasks");

    lst = tasks.childNodes();

    for(int i=0;i<lst.count();i++)
        comboBox->addItem(lst.at(i).attributes().namedItem("name").nodeValue());

    if(selected.isValid())
        comboBox->setCurrentIndex(selected.row());

    connect(pushButton,SIGNAL(clicked()),this,SLOT(slotGetFiles()));
}

AddFiles::~AddFiles()
{
}

void AddFiles::slotGetFiles()
{

    files = QFileDialog::getOpenFileNames(this,tr("Add Existing Files"),"",
                                          tr("Source files( *.cpp *.c *.h *.cl *.txt )"));


    QString str("");

    /**
     *  Show only file name , not entire path,
     *  so we get only the last token
     */
    Q_FOREACH(QString tmp, files)
    {
        str+="\"";
        str+= tmp.split(sep).last();
        str+="\" ";
    }

    lineEdit->setText(str);

}

void AddFiles::slotAddFiles()
{
    /**
     *  No need to search node by name,
     *  we put them in order in combobox
     *  so the index is just enough
     */
    QDomNode task = lst.at(comboBox->currentIndex());

    ProjectTreeItem* taskItem = tasksItem->child(comboBox->currentIndex());

    Q_FOREACH(QString file, files)
    {
        QDomElement elem = projectXml->createElement("file");
        elem.setAttribute("name",file.split(sep).last());
        task.appendChild(elem);
        parent->model->addItem(new ProjectTreeItem(elem,taskItem),taskItem);
    }

    copyFiles();

    this->close();
}

void AddFiles::copyFiles()
{
    QString tmp(parent->whcFile);

    /** remove . whc filename **/
         tmp = tmp.remove(tmp.split("/").last());

     /** got to src/$Task **/
     tmp.append("src/"+ comboBox->itemText(comboBox->currentIndex()) + "/");

     /** and copy all files **/
     Q_FOREACH(QString src, files)
     {
         /**
           * It seem that QFileDialog::getFileNames() give file paths
           * in Windows different than in Linux (slash vs backslash)
           * so we need a custom defined separator
           * note that QFileDialog::getFileName() doesn't have this problem
           */
         QFile::copy(src,tmp+src.split(sep).last());
     }

}
