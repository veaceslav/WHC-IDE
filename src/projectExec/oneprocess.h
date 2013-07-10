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

#ifndef ONEPROCESS_H
#define ONEPROCESS_H

#include <QObject>

#ifdef _WIN32
#define OS 1
#elif __linux__
#define OS  2
#endif

class CommandLine;
class QProcess;
class Node;
class ProjectTreeModel;
class QDomDocument;
class Ide;

/**
 * @brief The OneProcess class is designed to execute a task one time with
 *          one set of command line arguments, on one device
 *          It also watches if the task has data folders connected to output and
 *          copies the result from the temporary folder to data folders.
 *
 */
class OneProcess : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief OneProcess
     * @param cmd - Shared Command line that show execution progress
     * @param lst - command line arguments + buildpath at beginning
     * @param nod - Task Node that keeps info about outputs
     */
    explicit OneProcess(CommandLine *cmd, QStringList &lst, Node *nod,
                        ProjectTreeModel *model);
    ~OneProcess();
    /**
     * @brief startExecution - execute task with given arguments
     */
    void startExecution();

signals:

    /**
     * @brief signalEnd - emit this signal when both execution and data copy are
     *                    done, so the Execute class can go to next element in
     *                    queue
     */
    void signalEnd(int, int, QStringList *);

private slots:

    /**
     * @brief copytoData - after execution, check if data folders are connected
     *                     to output and copy data to them
     */
    void copytoData();

    void slotUpdateText();

    void slotUpdateError();

private:

    /**
     * @brief getExecutableName -  parse CMakeLists.txt for a task and return name of the executable
     * @param path - file that is parsed to get the name of the executable
     * @return - name of the executable
     *         - if the name of the executable si not found in CMakeLists.txt, then return ""
     */
    QString getExecutableName(QString path);

    QProcess *proc;
    Node *taskNode;
    CommandLine *cmdL;
    QStringList *args;
    QString buildPath;
    QString tempPath;

    ProjectTreeModel *model;
    QDomDocument *projXml;

    int device;
};

#endif // ONEPROCESS_H
