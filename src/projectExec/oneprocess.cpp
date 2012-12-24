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

#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QDomDocument>

#include "oneprocess.h"
#include "projBuild/commandline.h"
#include "sorttask.h"
#include "ide.h"
#include "model/projecttreemodel.h"


OneProcess::OneProcess(CommandLine* cmd, QStringList& lst, Node* nod,Ide* parent)
    :taskNode(nod),cmdL(cmd),list(lst),
     model(parent->model),projXml(parent->getProjectXml())
{
    proc = new QProcess();

    device = lst.last().toInt();
    buildPath = lst.first();
    tempPath = lst.at(4);

    //qDebug() << "Device id is" << device;
}

OneProcess::~OneProcess()
{
    proc->kill();
    delete proc;
}

void OneProcess::slotUpdateError()
{
    QByteArray data = proc->readAllStandardError();
    cmdL->addDebugLine(QString(data));
}

void OneProcess::slotUpdateText()
{
    QByteArray data = proc->readAllStandardOutput();
    cmdL->addDebugLine(QString(data));
}
void OneProcess::startExecution()
{

    proc->setWorkingDirectory(list.first());

    connect(proc, SIGNAL(readyReadStandardError()),
            this, SLOT(slotUpdateError()));
    connect(proc, SIGNAL(readyReadStandardOutput()),
            this, SLOT(slotUpdateText()));
    connect(proc,SIGNAL(finished(int)),
            this,SLOT(copytoData()));

    QString program;

    if(OS == 1)
        program = list.first() + "//" + taskNode->Name +".exe";
    if(OS == 2)
        program = "./" + taskNode->Name;

    list.removeFirst();

    cmdL->addLine(QString( "Executing "+taskNode->Name
                           + list.join(" ") + "\n"),Qt::darkGreen);
    proc->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc->start(program,list);
}


void OneProcess::copytoData()
{
    int inputs = taskNode->link.size()-5;
    if(taskNode->link[inputs].isEmpty())
    {
        emit signalEnd(device);
        return;
    }

    QDir dir(buildPath);
    QString source = dir.cleanPath(buildPath + "/" +tempPath);
    QString filename = tempPath.split("/").last();
    for(int i=0;i<taskNode->link[inputs].size();i++)
        if(taskNode->link[inputs].at(i)->type == 1) // Data type
        {
            Node* data = taskNode->link[inputs].at(i);
            QString dest = dir.cleanPath(buildPath
                    +"../../../../data/" + data->Name + "/" + filename);

            /** Add files to project tree **/
            ProjectTreeItem* groupItem = model->getGroupByName(data->Name);
            if(!groupItem->searchChildByName(filename))
            {
                QDomElement elem = projXml->createElement("file");
                elem.setAttribute("name",filename);

                QDomNode groupNode = groupItem->getNode();
                groupNode.appendChild(elem);

                model->addItem(new ProjectTreeItem(elem,groupItem),groupItem);
            }

            /** Copy file to data folder, overwrite**/
            if(QFile::exists(dest))
            {
                if(!QFile::remove(dest))
                    qDebug() << "copyToData: impossible to remove file";

            }
            if(!QFile::copy(source,dest))
                qDebug() << "copyToData: Error! file was not copied";
        }
    emit signalEnd(device);
}
