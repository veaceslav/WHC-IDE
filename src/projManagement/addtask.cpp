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

#include "projManagement/addtask.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QDomDocument>
#include <QDir>
#include <QTextStream>
#include <QDate>

#include "model/projecttreemodel.h"
#include "model/projecttreemodel.h"
#include "diagram/diagramwindow.h"

AddTask::AddTask(QDomDocument* proj, Ide *parent)
    : QDialog(parent)
{

    projectXml = proj;

    this->parent = parent;

    this->resize(391, 98);
    this->setWindowTitle(QString("Add New Task"));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setGeometry(QRect(40, 60, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    taskLabel = new QLabel(this);
    taskLabel->setGeometry(QRect(20, 10, 71, 21));
    taskLabel->setText("Task Name:");

    taskName = new QLineEdit(this);
    taskName->setObjectName(QString::fromUtf8("lineEdit"));
    taskName->setGeometry(QRect(100, 10, 271, 22));

    xLabel = new QLabel(this);
    xLabel->setGeometry(QRect(20, 40, 51, 21));
    xLabel->setText("Inputs:");

    xPoz = new QLineEdit(this);
    xPoz->setGeometry(QRect(100, 40, 81, 22));
    xPoz->setValidator(new QIntValidator(this));

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAddTask()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

AddTask::~AddTask()
{
}

void AddTask::slotAddTask()
{

    if(taskName->text().isEmpty())
    {
        QMessageBox msg;
        msg.setText("No task name specified.");
        msg.setInformativeText("Please enter task name.");
        msg.exec();
        return;
    }
    QString x = xPoz->text();
    QDomNode nod =projectXml->lastChild();

    QDomNode tasks = nod.namedItem("tasks");

    QDomElement tmp = projectXml->createElement("task");

    if(!x.isEmpty())
    {
        tmp.setAttribute("x",x.toInt());
    }
    tmp.setAttribute(QString("name"),taskName->text());
    int id = tasks.lastChild().attributes().namedItem("id").nodeValue().toInt()
             + 1;
    tmp.setAttribute("id",id);

    QDomText txt = projectXml->createTextNode("");

    tmp.appendChild(txt);
    tasks.appendChild(tmp);

    QModelIndex taskIndex = parent->model->tasksIndex();

    ProjectTreeItem* parentItem =
                     static_cast<ProjectTreeItem*>(taskIndex.internalPointer());
    ProjectTreeItem* addedTask = new ProjectTreeItem(tmp,parentItem);
    parent->model->addItem(addedTask,parentItem);

    /**
     *  Create a string holding the path to the task.
     */
    QString taskPath(parent->whcFile);
    taskPath.remove(taskPath.split("/").last());
    taskPath.append("src/" + taskName->text());

    QDir dir;
    /**
     *  Create task directory in src folder
     */
    dir.mkdir(taskPath);
    /**
     *  Create Execute directory in task folder
     */
    dir.mkdir(taskPath + "/Execute");

    bool mainExists = QFile::exists(taskPath + "/main.cpp");

    if (!mainExists)
        mainExists = generateMainFile(taskPath + "/main.cpp", x.toInt());

    /**
     * If the program finished generating the main.cpp file or if it was
     * already on the drive, the file will be added to the project tree.
     */
    if(mainExists)
    {
        QDomElement elem = projectXml->createElement("file");
        elem.setAttribute("name","main.cpp");
        tmp.appendChild(elem);
        parent->model->addItem(new ProjectTreeItem(elem, addedTask), addedTask);
    }
    else
        qDebug() << "Could not generate main.cpp file in folder " << taskPath;

    /**
     *  Add created task to diagram comboBox
     */
    parent->getDiagram()->addTask(taskName->text(), x.toInt(), 1, id);

    this->close();
}

bool AddTask::generateMainFile(QString path, int inputs)
{
    QFile mainTemplate(":/mainTemplate.cpp");
    QTextStream input(&mainTemplate);
    QString templateText = "";

    if(mainTemplate.open(QIODevice::ReadOnly))
    {
        templateText = input.readAll();
        mainTemplate.close();
        /**
          * Replace the parts tagged with ` ` with user specific data.
          */
        templateText.replace("`TASKNAME`", taskName->text());
        templateText.replace("`DATE`",
                                   QDate::currentDate().toString("dd.MM.yyyy"));
        templateText.replace("`ARGNO`", QString::number(inputs + 6));
    }
    else
    {
        /**
          * This shouldn't happen. The mainTemplate.cpp file should always be
          * available.
          */
        qDebug() << "generateMainFile(QString,int) could not open mainTemplate";
    }

    QFile mainCpp(path);
    QTextStream output(&mainCpp);

    /**
      * Opening a file in write mode will create it if it doesn't exist.
      */
    if(mainCpp.open(QIODevice::WriteOnly))
    {
        output << templateText;
        mainCpp.close();
        return true;
    }
    else
    {
        /**
          * There was a problem with opening the main.cpp file, so the task
          * will not have an autogenerated main.cpp file.
          */
        return false;
    }
}
