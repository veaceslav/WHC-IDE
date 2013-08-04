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


OneProcess::OneProcess(CommandLine *cmd, QStringList *lst,
                       Node *nod, ProjectTreeModel *model)
    :taskNode(nod), cmdL(cmd), args(lst), model(model),
      projXml(model->getProjectXml())
{
    proc = new QProcess();
    device = lst->last().toInt();
    buildPath = lst->first();
    tempPath = lst->at(lst->size() - 3);
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

void OneProcess::slotProcessFailed(QProcess::ProcessError error)
{
    QString errType;
    switch(error)
    {
    case QProcess::FailedToStart :
        errType = "failed to start";
        break;
    case QProcess::Crashed :
        errType = "crashed";
        break;
    case QProcess::Timedout :
        errType = "timedout";
        break;
    case QProcess::WriteError :
        errType = "write error";
        break;
    case QProcess::ReadError :
        errType = "read error";
        break;
    case QProcess::UnknownError :
        errType = "unknown error";
        break;
    }
    writeErrorToCmd("process error - " + errType);


    emit signalEnd(device, taskNode->diagId, args, ProcessError, error);
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
    connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(slotCopyToData(int, QProcess::ExitStatus)));
    connect(proc, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(slotProcessFailed(QProcess::ProcessError)));


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

void OneProcess::copyToData()
{
    int outputs = taskNode->link.size() - 1;
    if(!taskNode->link[outputs].isEmpty())
    {
        QDir dir(buildPath);
        QString source = dir.cleanPath(buildPath + "/" + tempPath);
        QString filename = tempPath.split("/").last();

        QString message = "[Device %1] Copying output...";
        cmdL->addLine(message.arg(device), Qt::darkGreen);

        for(int i = 0; i < taskNode->link[outputs].size(); i++)
            if(taskNode->link[outputs].at(i)->type == 1) // Data type
            {
                Node *data = taskNode->link[outputs].at(i);

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

                    model->addItem(new ProjectTreeItem(elem, groupItem),
                                   groupItem);
                }

                /** Copy file to data folder, overwrite**/
                if(QFile::exists(dest) && !QFile::remove(dest))
                {
                    qDebug() << "copyToData: impossible to overwrite old file";
                    writeErrorToCmd("overwrite failed");

                    emit signalEnd(device, taskNode->diagId, args,
                                   IOError, Replace);
                    return;
                }


                /** Create output folder if not present **/
                if(!dir.exists(destDir) && !dir.mkdir(destDir))
                {
                    qDebug() << "copyToData: cannot create output directory";
                    writeErrorToCmd("output folder creation failed");

                    emit signalEnd(device, taskNode->diagId, args,
                                   IOError, Mkdir);
                    return;
                }

                if(!QFile::copy(source, dest))
                {
                    qDebug() << "copyToData: Error! file was not copied";
                    writeErrorToCmd("output copy failed");

                    emit signalEnd(device, taskNode->diagId, args,
                                   IOError, Copy);
                    return;
                }
            }
    }

    cmdL->addLine(QString("[Device %1] Done!").arg(device), Qt::darkGreen);
    emit signalEnd(device, taskNode->diagId, args, Success);
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
        /** search for the line that contains "add_executable(" **/
        if (line.contains("add_executable("))
        {
            listOfWords = line.split("(");
            line = listOfWords[1];
            listOfWords = line.split(" ");
            file.close();
            /** return name of the executable **/
            return listOfWords[0];
        }
    }while(!in.atEnd());

    file.close();
    /**
     * return "" if the name of the executable was not found in the parsed file
     */
    return QString("");
}

void OneProcess::writeErrorToCmd(QString message)
{
    QString msg = QString("[Device %1] Error: ").arg(device) + message;
    cmdL->addLine(msg, Qt::red);
}

void OneProcess::slotCopyToData(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString message = "[Device %1] Process ended with exit code %2";
    cmdL->addLine(message.arg(device).arg(exitCode), Qt::darkGreen);

    if(exitStatus == QProcess::CrashExit)
    {
        writeErrorToCmd("crash exit");

        emit signalEnd(device, taskNode->diagId, args, CrashExitError);
        return;
    }

    copyToData();
}
