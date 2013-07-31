/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        : 23 JUL 2013
 * Description : Monitor class, responsible for logging and statistics.
 *
 * Copyright (C) 2013 by Preda Andrei <andreipreda07 at gmail dot com>
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

#ifndef MONITOR_H
#define MONITOR_H

#include <QSettings>
#include <QTime>
#include <QMap>

/**
 * The total number of processes that ran on the device
 */
#define DEV_RUNS "devId%1_status%2_info%3"

/**
 * The total number of runs
 */
#define PROJ_RUNS "projRuns"

/**
 * The total number of processes that ran;
 */
#define PROCS_RAN "procsRan"

/**
 * The device on which the process ran
 */
#define PROC_DEVID "procDevId_Id%1"

/**
 * The time it took to finish the process
 */
#define PROC_TIME "procTime_Id%1"

/**
 * The input file/s for the process
 **/
#define PROC_INPUT "procInput_Id%1"

/**
 * The id of the process in the workflow diagram
 */
#define PROC_DIAG_ID "procDiagId_Id%1"

/**
 * The status of the process (Succress, IOError, Crash exit or Process error)
 */
#define PROC_EXIT_STATUS "procExitStatus_Id%1"

/**
 * Additional info regarding the process exit status
 */
#define PROC_EXIT_INFO "procExitInfo_Id%1"

/**
 * Total execution time
 */
#define EXEC_TIME "execTime"

class Monitor : public QObject
{
    Q_OBJECT
public:
    Monitor();
    ~Monitor();

public slots:
    void slotStartProcess(int devId);
    void slotStartExecute(QString whcFile);

    void slotFinishedProcess(int devId, int taskId, QString *inFiles,
                             int taskStatus, int moreInfo);
    void slotFinishedExecute();

private:
    /**
     * @brief aggregateStats - the stats of all the projects, combined
     */
    QSettings *aggregateStats;
    /**
     * @brief projectStats - the stats of the current project, contains number
     *                       of runs
     */
    QSettings *projectStats;
    /**
     * @brief projectStatsPart - a part of the project stats that contains info
     *                           about a single run
     */
    QSettings *projectStatsPart;
    /**
     * @brief runStats - the stats of the last run
     */
    QSettings *runStats;

    int procsRan, runId;

    QTime execTimer;
    QMap<int, QTime *> procTimer;
};

#endif // MONITOR_H
