/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        : 23 JUL 2013
 * Description : Implementation for the monitor class, responsible for logging.
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

#include <QStringList>
#include <QString>
#include <QDir>
#include <QDebug>

#include "monitor.h"
#include "projectExec/oneprocess.h"

Monitor::Monitor()
{
    aggregateStats = new QSettings("WHC", "WHC IDE Monitor");
}

Monitor::~Monitor()
{
    delete aggregateStats;
}

void Monitor::slotStartProcess(int devId)
{
    procTimer[devId] = new QTime();
    procTimer[devId]->start();
}

void Monitor::slotStartExecute(QString whcFile)
{
    QString logPath = whcFile.remove(whcFile.split("/").last());
    QDir projectDir(logPath);
    projectDir.mkdir("log");

    projectStats = new QSettings(logPath + "log/stats" , QSettings::IniFormat);
    runStats = new QSettings(logPath + "log/run", QSettings::IniFormat);

    if(!projectStats->contains(PROJ_RUNS))
        runId = 1;
    else
        runId = projectStats->value(PROJ_RUNS).toInt() + 1;

    projectStatsPart = new QSettings(logPath +
                                     QString("log/stats%1").arg(runId),
                                     QSettings::IniFormat);

    runStats->clear();
    procsRan = 0;

    execTimer.start();
}

void Monitor::slotFinishedProcess(int devId, int taskId, QString *inFiles,
                                  int taskStatus, int moreInfo)
{
    int timeElapsed = procTimer[devId]->elapsed();

    runStats->setValue(QString(PROC_DEVID).arg(procsRan), devId);
    runStats->setValue(QString(PROC_DIAG_ID).arg(procsRan), taskId);
    runStats->setValue(QString(PROC_TIME).arg(procsRan), timeElapsed);
    runStats->setValue(QString(PROC_INPUT).arg(procsRan), *inFiles);
    runStats->setValue(QString(PROC_EXIT_STATUS).arg(procsRan), taskStatus);
    runStats->setValue(QString(PROC_EXIT_INFO).arg(procsRan), moreInfo);

    projectStatsPart->setValue(QString(PROC_DEVID).arg(procsRan), devId);
    projectStatsPart->setValue(QString(PROC_DIAG_ID).arg(procsRan), taskId);
    projectStatsPart->setValue(QString(PROC_TIME).arg(procsRan), timeElapsed);
    projectStatsPart->setValue(QString(PROC_INPUT).arg(procsRan), *inFiles);
    projectStatsPart->setValue(QString(PROC_EXIT_STATUS).arg(procsRan),
                               taskStatus);
    projectStatsPart->setValue(QString(PROC_EXIT_INFO).arg(procsRan), moreInfo);

    /**
     * @brief devProcRuns - the key of the setting that counts the number of
     *                      processes that ran on this device with the given
     *                      status and the given additional info
     */
    QString devProcRuns =
            QString(DEV_RUNS).arg(devId).arg(taskStatus).arg(moreInfo);
    int ranOnDev;
    if(!aggregateStats->contains(devProcRuns))
        ranOnDev = 1;
    else
        ranOnDev = aggregateStats->value(devProcRuns).toInt() + 1;

    aggregateStats->setValue(devProcRuns, ranOnDev);
    procsRan++;

    delete inFiles;
    if(taskStatus != OneProcess::ProcessError)
        delete procTimer[devId];
}

void Monitor::slotFinishedExecute()
{
    int timeElapsed = execTimer.elapsed();

    runStats->setValue(EXEC_TIME, timeElapsed);
    runStats->setValue(PROCS_RAN, procsRan);

    projectStatsPart->setValue(EXEC_TIME, timeElapsed);
    projectStatsPart->setValue(PROCS_RAN, procsRan);
    projectStats->setValue(PROJ_RUNS, runId);

    aggregateStats->sync();
    projectStats->sync();
    projectStatsPart->sync();
    runStats->sync();

    delete projectStats;
    delete projectStatsPart;
    delete runStats;
}
