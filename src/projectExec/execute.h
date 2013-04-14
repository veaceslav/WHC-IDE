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
#include <QStringList>
#include <QMap>
#include <QDir>
#include "oneprocess.h"

class Node;
class QProcess;
class QTextEdit;
class QToolButton;
class Ide;
class CommandLine;

class Execute : public QObject
{
    Q_OBJECT
public:
    Execute(QString whcFile, QVector<Node*> sorted, QVector<int> devices,
            Ide *parent, CommandLine *cmd);

    void stopExec();

    void forceStop();

private slots:
    /**
     * @brief slotNextProcess - after finishing, calls one again start()
     *                          to execute next element
     */
    void slotNextProcess(int dev);

signals:

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

    /**
     * @brief start    pop one element from queue and execute it. It's called
     *                 until queue is empty
     */
    void start(int devId);

    QString path;
    int taskIndex;

    QQueue<QPair<Node*, QStringList> > q;
    QVector<Node*> execOrder;

    Ide *parent;
    CommandLine *cmd;

    OneProcess *exec;
    QMap<int, OneProcess*> exec2;

    bool stop;
    int devCount;
    int devFinished;
};
#endif // EXECUTE_H
