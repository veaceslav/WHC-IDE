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

#include "projManagement/adddata.h"
#include <QApplication>
#include <model/projecttreemodel.h>
#include <QLineEdit>
#include <QFileDialog>

AddData::AddData(QDomDocument* proj, Ide* parent, QModelIndex selected)
    :QDialog(parent)
{
    Q_UNUSED(proj);

    projectXml = proj;
    this->parent = parent;

    dataItem = parent->model->getItem(parent->model->dataIndex());

    this->resize(390, 148);
    this->setWindowTitle("Add Existing Data Files");

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    label = new QLabel(this);
    label->setText("Choose file(s):");

    lineEdit = new QLineEdit(this);

    label_2 = new QLabel(this);
    label_2->setText("Choose Group:");

    comboBox = new QComboBox(this);

    pushButton = new QPushButton(this);
    pushButton->setText("...");

    gridLayout.addWidget(label, 0, 0);
    gridLayout.addWidget(lineEdit, 0, 1);
    gridLayout.addWidget(pushButton, 0, 2);
    gridLayout.addWidget(label_2, 1, 0);
    gridLayout.addWidget(comboBox, 1, 1);
    orizontalLayout.addWidget(buttonBox);
    verticalLayout.addLayout(&gridLayout);
    verticalLayout.addLayout(&orizontalLayout);
    verticalLayout.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setLayout(&verticalLayout);


    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAddData()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


    QDomNode nod =proj->lastChild();

    QDomNode groups = nod.namedItem("data");

    lst = groups.childNodes();

    for(int i=0;i<lst.count();i++)
        comboBox->addItem(lst.at(i).attributes().namedItem("name").nodeValue());

    /** Set as default the group that was selected when option was activated **/
    if(selected.isValid())
        comboBox->setCurrentIndex(selected.row());
    connect(pushButton,SIGNAL(clicked()),this,SLOT(slotGetFiles()));
}

AddData::~AddData()
{
}

void AddData::slotGetFiles()
{

    files = QFileDialog::getOpenFileNames(this,tr("Add Existing Files"),"");


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

void AddData::slotAddData()
{
    /**
     *  No need to search node by name,
     *  we put them in order in combobox
     *  so the index is just enough
     */
    QDomNode task = lst.at(comboBox->currentIndex());
    ProjectTreeItem* groupItem = dataItem->child(comboBox->currentIndex());

    Q_FOREACH(QString file, files)
    {
        QDomElement elem = projectXml->createElement("file");
        elem.setAttribute("name",file.split(sep).last());
        task.appendChild(elem);

        parent->model->addItem(new ProjectTreeItem(elem,groupItem),groupItem);
    }

    copyFiles();

    this->close();
}

void AddData::copyFiles()
{
    QString tmp(parent->whcFile);

    /** remove . whc filename **/
         tmp = tmp.remove(tmp.split("/").last());

     /** got to src/$Task **/
     tmp.append("data/"+ comboBox->itemText(comboBox->currentIndex()) + "/");

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
