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


OneProcess::OneProcess(CommandLine *cmd, QStringList &lst,
                       Node *nod, ProjectTreeModel *model)
    :taskNode(nod), cmdL(cmd), model(model), projXml(model->getProjectXml())
{
    proc = new QProcess();
    args = new QStringList(lst);
    device = lst.last().toInt();
    buildPath = lst.first();
    tempPath = lst.at(4);

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

    proc->setWorkingDirectory(args->first());

    connect(proc, SIGNAL(readyReadStandardError()),
            this, SLOT(slotUpdateError()));
    connect(proc, SIGNAL(readyReadStandardOutput()),
            this, SLOT(slotUpdateText()));
    connect(proc, SIGNAL(finished(int)),
            this, SLOT(copytoData()));


    QString executableName = buildPath;
    executableName.remove(QString("/build"));
    executableName = getExecutableName(executableName + "/CMakeLists.txt");

    if (executableName == "" && OS == 1)
        executableName = taskNode->Name + ".exe";
    else if (executableName == "" )
        executableName = taskNode->Name;

    QString program;

    if(OS == 1)
        program = args->first() + "//" + executableName;
    if(OS == 2)
        program = "./" + executableName;

    args->removeFirst();

    cmdL->addLine(QString( "Executing " + taskNode->Name + " "
                           + args->join(" ") + "\n"), Qt::darkGreen);
    proc->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    proc->start(program, *args);
}

QString OneProcess::getExecutableName(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return QString("");

    QString line;
    QStringList listOfWords;
    QTextStream in(&file);

    do
    {
        line = in.readLine();
        // search for the line that contains "add_executable("
        if (line.contains("add_executable("))
        {
            listOfWords = line.split("(");
            line = listOfWords[1];
            listOfWords = line.split(" ");
            file.close();
            // return name of the executable
            return listOfWords[0];
        }
    }while(!in.atEnd());

    file.close();
    // return "" if the name of the executable was not found in the parsed file
    return QString("");
}

void OneProcess::copytoData()
{
    int inputs = taskNode->link.size() - 1;
    if(taskNode->link[inputs].isEmpty())
    {
        emit signalEnd(device, taskNode->diagId, args);
        return;
    }

    QDir dir(buildPath);
    QString source = dir.cleanPath(buildPath + "/" + tempPath);
    QString filename = tempPath.split("/").last();
    for(int i = 0; i < taskNode->link[inputs].size(); i++)
        if(taskNode->link[inputs].at(i)->type == 1) // Data type
        {
            Node *data = taskNode->link[inputs].at(i);

            QString destDir = dir.cleanPath(buildPath
                                            + "../../../../data/" + data->Name);
            QString dest = dir.cleanPath(destDir + "/" + filename);

            /** Add files to project tree **/
            ProjectTreeItem *groupItem = model->getGroupByName(data->Name);
            if(!groupItem->searchChildByName(filename))
            {
                QDomElement elem = projXml->createElement("file");
                elem.setAttribute("name", filename);

                QDomNode groupNode = groupItem->getNode();
                groupNode.appendChild(elem);

                model->addItem(new ProjectTreeItem(elem, groupItem), groupItem);
            }

            /** Copy file to data folder, overwrite**/
            if(QFile::exists(dest))
            {
                if(!QFile::remove(dest))
                    qDebug() << "copyToData: impossible to remove file";

            }

            /** Create output folder if not present **/
            if (!dir.exists(destDir))
                dir.mkdir(destDir);

            if(!QFile::copy(source, dest))
                qDebug() << "copyToData: Error! file was not copied";
        }
    emit signalEnd(device, taskNode->diagId, args);
}
