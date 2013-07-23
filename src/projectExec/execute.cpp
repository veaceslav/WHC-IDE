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

#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QToolButton>
#include <QTextEdit>
#include <QLinkedList>

#include "projBuild/commandline.h"
#include "execute.h"
#include "sorttask.h"

Execute::Execute(QString whcFile, QVector<Node*> sorted, QVector<int> devices,
                 Ide *parent, CommandLine *cmd,
                 QLinkedList<Exclusion> exclusionList):execOrder(sorted),
                 devices(devices), exclusions(exclusionList), cmd(cmd)
{
    path = whcFile.remove(whcFile.split("/").last());

    if(parent->mustSaveFlow())
    {
        QDir logDir(path);
        logDir.mkdir("log");

        saveExecProgress = new QFile(path + "/log/flow");
        saveExecProgress->open(QIODevice::WriteOnly);

        saveStream = new QTextStream(saveExecProgress);
    }
    else
    {
        saveExecProgress = NULL;
    }

    if(parent->mustLog())
    {
        monitor = new Monitor();

        connect(this, SIGNAL(signalStartedExec(QString)),
                monitor, SLOT(slotStartExecute(QString)));
        connect(this, SIGNAL(signalStartedProc(int)),
                monitor, SLOT(slotStartProcess(int)));

        connect(this, SIGNAL(signalFinishedExec()),
                monitor, SLOT(slotFinishedExecute()));
        connect(this, SIGNAL(signalFinishedProc(int, int, QString*, int, int)),
                monitor,
                SLOT(slotFinishedProcess(int, int, QString*, int, int)));

        emit signalStartedExec(whcFile);
    }
    else
    {
        monitor = NULL;
    }

    this->parent = parent;
    taskIndex    = 0;
    stop         = false;

    devCount     = devices.count();
    devFinished  = 0;

    for(int i = 0; i < devices.size(); i++)
        exec2[devices[i]] = NULL;

    connect(this, SIGNAL(signalFinishedExec()),
              parent, SLOT(slotFinishedExec()));

    connect(this, SIGNAL(signalRecovered(int,int,QStringList*,int,int)),
              this, SLOT(slotNextProcess(int,int,QStringList*,int,int)));

    cmd->showM();
    execute();
}

Execute::~Execute()
{
    if(saveExecProgress)
    {
        saveExecProgress->close();
        delete saveExecProgress;
        delete saveStream;
    }
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

void Execute::slotNextProcess(int dev, int finishedTask, QStringList *args,
                              int taskStatus, int moreInfo)
{
    if(saveExecProgress &&
       (taskStatus == OneProcess::Success || taskStatus == OneProcess::IOError))
    {
        (*saveStream)<<finishedTask<<" ";
        /**
         * size() - 4 excludes the -out outputFile -dev Id arguments
         */
        for(int i = 1; i < args->size() - 4; i++)
            (*saveStream)<<args->at(i)<<" ";
        (*saveStream)<<QString("-status %1 %2\n").arg(taskStatus).arg(moreInfo);
        saveStream->flush();
    }

    if(monitor)
    {
        QString *inFiles = new QString();
        for(int i = 1; i < args->size() - 4; i++)
            (*inFiles) += args->at(i) + "//";
        emit signalFinishedProc(dev, finishedTask, inFiles, taskStatus,
                                moreInfo);
    }

    delete args;
    exec2[dev]->deleteLater();

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

    for(int i = 0; i < devices.size(); i++)
        start(i);
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

bool Execute::tryRecover(int devId, QStringList *list,
                         QPair<Node*, QStringList> pair)
{
    for(QLinkedList<Exclusion>::Iterator i = exclusions.begin();
        i != exclusions.end(); i++)
    {
        if(i->taskId != pair.first->diagId)
            continue;

        if(i->taskStatus != OneProcess::Success &&
           i->taskStatus != OneProcess::IOError)
            continue;

        /**
         * list.size() - 6 is the number of input files. The 6 strings that
         * were substracter are: executable name, "-in", "-out", output file,
         * "-dev", device id.
         */
        if(i->inFiles.size() != list->size() - 6)
            continue;

        bool foundDiff = false;
        for(int j = 0; j < i->inFiles.size(); j++)
            /**
             * list has an offset of 2, the first two strings being
             * executable name and "-in".
             */
            if(i->inFiles[j] != (*list)[j + 2])
            {
                foundDiff = true;
                break;
            }
        if(foundDiff)
            continue;

        /**
         * The full executable name is not relevant to the user, so it will not
         * be printed on cmd.
         */
        list->removeFirst();

        if(i->taskStatus == OneProcess::Success)
        {
            cmd->addLine("Recovered " + pair.first->Name + " " +
                         list->join(" "), Qt::darkGreen);
        }
        else // i->taskStatus == OneProcess::IOError
        {
            cmd->addLine("Recovering " + pair.first->Name + " " +
                         list->join(" "), Qt::darkGreen);
            exec2[devId]->copyToData();
        }
        /**
         * The task has been recovered so we will remove it from the list of
         * tasks that can be recovered (skipped).
         */
        exclusions.erase(i);

        emit signalRecovered(devId, pair.first->diagId, list,
                             (int) OneProcess::Success, 0);
        return true;
    }
    return false;
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

    QStringList *listCopy = new QStringList(list);

    exec2[devId] = new OneProcess(cmd, listCopy, pair.first, parent->model);

    if(tryRecover(devId, listCopy, pair))
        return;

    connect(exec2[devId], SIGNAL(signalEnd(int, int, QStringList *, int, int)),
            this, SLOT(slotNextProcess(int, int, QStringList *, int, int)));

    if(monitor)
        emit signalStartedProc(devId);

    exec2[devId]->startExecution();

}
