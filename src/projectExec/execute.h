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

#ifndef EXECUTE_H
#define EXECUTE_H

#include <QVector>
#include <QString>
#include <QQueue>
#include <QObject>
#include <QPair>
#include <QMap>
#include <QDir>
#include <QLinkedList>
#include <QTextStream>
#include "oneprocess.h"

class Node;
class QProcess;
class QTextEdit;
class QToolButton;
class Ide;
class CommandLine;

struct Exclusion
{
    int taskId, taskStatus, moreInfo;
    QStringList inFiles;
};

class Execute : public QObject
{
    Q_OBJECT
public:
    Execute(QString whcFile, QVector<Node*> sorted, QVector<int> devices,
            Ide *parent, CommandLine *cmd,
            QLinkedList<Exclusion> exclusionList);

    ~Execute();

    void stopExec();

    void forceStop();

private slots:
    /**
     * @brief slotNextProcess - after finishing, calls one again start()
     *                          to execute next element
     */
    void slotNextProcess(int dev, int finishedTask, QStringList *args,
                         int taskStatus, int moreInfo);

signals:

    void signalRecovered(int dev, int finishedTask, QStringList *args,
                         int taskStatus, int moreInfo = 0);
    void signalFinishedExec();
    //maybe dead...
    void signaldataGained();

private:

    /**
     * @brief execute - prepare and execute tasks according to execOrder
     * @param order   - Execution Order, vector contains items sorted by
     *                  priority
     */
    void execute();

    /**
     * @brief fillQueue - add all info about task to be executed and command
     *                    line arguments for each data file from data folder
     * @param nod       - Task's node, used to get info from input connectors
     *                    about data folders
     */
    void fillQueue(Node *nod);

    bool tryRecover(int devId, QStringList *list,
                    QPair<Node*, QStringList> pair);

    /**
     * @brief start    pop one element from queue and execute it. It's called
     *                 until queue is empty
     */
    void start(int devId);

    QString path;
    int taskIndex;

    QQueue<QPair<Node*, QStringList> > q;
    QVector<Node*> execOrder;
    QVector<int> devices;
    QLinkedList<Exclusion> exclusions;

    Ide *parent;
    CommandLine *cmd;

    /**
     * @brief execProgress    file used to save the execution workflow.
     */
    QFile *saveExecProgress;
    QTextStream *saveStream;

    QMap<int, OneProcess*> exec2;

    bool stop;
    int devCount;
    int devFinished;
};
#endif // EXECUTE_H
