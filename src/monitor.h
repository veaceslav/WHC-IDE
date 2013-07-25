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
     * @brief projectStats - the stats of the current project
     */
    QSettings *projectStats;
    /**
     * @brief runStats - the stats of the last run
     */
    QSettings *runStats;

    int procsRan, runId;

    QTime execTimer;
    QMap<int, QTime *> procTimer;
};

#endif // MONITOR_H
