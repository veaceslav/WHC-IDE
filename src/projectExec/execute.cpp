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

#include "execute.h"
#include "sorttask.h"
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QToolButton>
#include <QTextEdit>
#include "projBuild/commandline.h"

Execute::Execute(QString whcFile, QVector<Node*> sorted, QVector<int> devices,
                 Ide *parent, CommandLine *cmd):execOrder(sorted), cmd(cmd)
{
    path = whcFile.remove(whcFile.split("/").last());

    this->parent = parent;
    taskIndex    = 0;
    stop         = false;

    devCount     = devices.count();
    devFinished  = 0;

    for(int i = 0; i < devices.size(); i++)
        exec2[devices[i]] = 0;

    connect(this, SIGNAL(signalFinishedExec()),
              parent, SLOT(slotFinishedExec()));

    cmd->showM();
    execute();
}

Execute::~Execute()
{
}

void Execute::stopExec()
{
    stop = true;
    cmd->addLine("Stopping... Waiting for unfinished jobs.", Qt::red);
    emit signalFinishedExec();
}

void Execute::forceStop()
{
    exec2.clear();
    stop = true;
    cmd->addLine("Force Stop! Note: Data can be corrupted!", Qt::red);
    emit signalFinishedExec();
}

void Execute::slotNextProcess(int dev)
{
    cmd->addLine("Done!", Qt::darkGreen);
    if(!stop)
        start(dev);

}

void Execute::execute()
{

    if(stop)
        return;

    while(taskIndex < execOrder.size() && execOrder[taskIndex]->type != 0)
        taskIndex++;

    if(taskIndex >= execOrder.count())
    {
        /**
          * Finished running all tasks.
          */
        emit signalFinishedExec();
        return;
    }

    fillQueue(execOrder[taskIndex]);
    taskIndex++;

    /** Reset the number of device that finished counter
     *  and start execution on all selected devices
     */
    devFinished = 0;
    QMap<int, OneProcess*>::iterator it;

    for(it = exec2.begin(); it != exec2.end(); ++it)
        start(it.key());
}

void Execute::fillQueue(Node *nod)
{
    if(stop)
        return;

    int inputs = nod->link.size() - 1;

    QVector<QStringList> v(inputs);

    for(int i = 0; i < inputs; i++)
    {
        for(int j = 0; j < nod->link[i].size(); j++)
        {
            Node* tmp = nod->link[i][j];
            if(tmp->type == 1) // Data
            {
                QString dataD = path + "data/" + tmp->Name + "/";
                QDir tmpDir(dataD);
                QStringList lst = tmpDir.entryList(QDir::Files);
                for(int k = 0; k < lst.size(); k++)
                    v[i] << QString("../../../data/"
                                    + tmp->Name + "/" + lst.at(k));
            }
            if(tmp->type == 0) // Task, temporary folder
            {
                QString taskD = path + "src/" + tmp->Name + "/Execute/temp_"
                        + QString::number(tmp->diagId) + "/";
                QDir tmpDir(taskD);
                QStringList lst = tmpDir.entryList(QDir::Files);
                for(int k = 0; k < lst.size(); k++)
                    v[i] << QString("../../../src/"
                                    + tmp->Name + "/Execute/temp_"
                                    + QString::number(tmp->diagId)
                                    + "/" + lst.at(k));
            }
        }
    }

    /**
     * Get minimum from all data sources or program will crash if supplied with
     * not enough arguments
     */

    int min = v[0].size();
    for(int i = 0; i < v.size(); i++)
        if(min > v[i].size())
            min = v[i].size();

    QString tempPath;
    QString dirPath = path + "src/" + nod->Name + "/Execute";
    QDir dir;


    // Check if /Execute folder exists and if not exists it is created
    if (!dir.exists(dirPath))
        dir.mkdir(dirPath);

    dirPath += "/temp_" + QString::number(nod->diagId);
    dir.mkdir(dirPath);


    tempPath = "../Execute/temp_" + QString::number(nod->diagId);

    for(int i = 0; i < min; i++)
    {
        QStringList lst;

        lst << QString(path + "src/" + nod->Name + "/build");
        lst << "-in";
        for(int j = 0; j < v.size(); j++)
            lst << v[j][i];
        lst << "-out";

        /** Output file name is pathToDataFolder + out_TaskName_diagId_index **/
        QString sourcePath = tempPath
                                   + "/out_" + nod->Name + "_"
                                   + QString::number(nod->diagId)
                                   + "_" + QString::number(i);
        lst << sourcePath;
        lst << "-dev";

        q.enqueue(qMakePair(nod, lst));
    }
}

void Execute::start(int devId)
{
    if(stop)
        return;

    if(q.isEmpty())
    {
        /** Waiting all devices to finish or data can be lost
         *  when refilling the execution queue
         */
        devFinished++;
        if(devFinished == devCount)
        {
            execute();
        }
        return;
    }

    QPair<Node*, QStringList> pair = q.dequeue();

    QStringList list = pair.second;

    list << QString::number(devId);

    if(exec2[devId])
        delete exec2[devId];

    exec2[devId] = new OneProcess(cmd, list, pair.first, parent->model);

    connect(exec2[devId], SIGNAL(signalEnd(int)),
            this, SLOT(slotNextProcess(int)));

    exec2[devId]->startExecution();

}
