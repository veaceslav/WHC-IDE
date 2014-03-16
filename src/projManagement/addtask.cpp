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
#include "staticmethods.h"
#include "model/projecttreemodel.h"
#include "model/projecttreemodel.h"
#include "diagram/diagramwindow.h"

AddTask::AddTask(QDomDocument* proj, Ide *parent) : QDialog(parent)
{

    projectXml = proj;

    this->parent = parent;

    this->resize(391, 98);
    this->setWindowTitle(QString("Add New Task"));


    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    taskLabel = new QLabel(this);
    taskLabel->setText("Task Name:");

    taskName = new QLineEdit(this);
    taskName->setObjectName(QString::fromUtf8("lineEdit"));

    xLabel = new QLabel(this);
    xLabel->setText("Inputs:");

    xPoz = new QLineEdit(this);
    xPoz->setValidator(new QIntValidator(this));

    gridLayout.addWidget(taskLabel, 0, 0);
    gridLayout.addWidget(taskName, 0, 1);
    gridLayout.addWidget(xLabel, 1, 0);
    gridLayout.addWidget(xPoz, 1, 1);
    orizontalLayout.addWidget(buttonBox);
    verticalLayout.addLayout(&gridLayout);
    verticalLayout.addLayout(&orizontalLayout);
    verticalLayout.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setLayout(&verticalLayout);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAddTask()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

AddTask::~AddTask()
{
    StaticMethods::destroyObj(&buttonBox);
    StaticMethods::destroyObj(&taskLabel);
    StaticMethods::destroyObj(&taskName);
    StaticMethods::destroyObj(&xLabel);
    StaticMethods::destroyObj(&xPoz);
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
        /**
          * Generates a part of the comment section (the part with the input
          * file names).
          */
        QString inArg = " *        argv[%1]     %2- %3 input file-name;\n";
        QString fileArgs = "";
        QStringList suffixes;
        suffixes << "th" << "st" << "nd" << "rd";
        for(int i = 1; i <= inputs && i <= MAX_NAMES_NO; i++)
        {
            int chosenSuffix = ((i % 10) >= 1 && (i % 10) <= 3
                                         && (i / 10) != 1) ? (i % 10) : 0;
            fileArgs += inArg.arg(QString::number(i + 1),
                                  (i + 1 < 10) ? "  " : " ", QString::number(i)
                                                   + suffixes.at(chosenSuffix));
        }
        if(inputs > MAX_NAMES_NO)
            fileArgs += " *        .......\n";

        /**
          * Generates a part of the printf that shows the usage.
          */
        QString inFileName = "in%1.txt ";
        QString inFiles = "";
        for(int i = 1; i <= inputs && i <= MAX_NAMES_NO; i++)
            inFiles += inFileName.arg(QString::number(i));
        if(inputs > MAX_NAMES_NO)
            inFiles += "... ";

        templateText = input.readAll();
        mainTemplate.close();
        /**
          * Replace the parts tagged with ` ` with user specific data.
          */
        templateText.replace("`TASKNAME`", taskName->text());
        templateText.replace("`DATE`",
                                   QDate::currentDate().toString("dd.MM.yyyy"));
        templateText.replace("`EXPAND'", fileArgs);
        for(int i = 1; i <= 4; i++)
            templateText.replace("`ARGNO" + QString::number(i) + "`",
                                               QString::number(inputs + 6 - i));
        templateText.replace("`ARGNO`", QString::number(inputs + 6));
        templateText.replace("`INFILES`", inFiles);
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
