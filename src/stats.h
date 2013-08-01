/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        : 27 JUL 2013
 * Description : Graphs and statistics regarding performance
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

#ifndef STATS_H
#define STATS_H

#include <QString>

#include "projectExec/devicequery.h"
#include "ui_stats.h"

class Stats : public QDialog
{
    Q_OBJECT
public:
    Stats(DeviceQuery *devQuery);
    Stats(DeviceQuery *devQuery, QString whcFile);
    ~Stats();
private:
    void initGraphs();
    void getRunData();
    void getGeneralData();
    void setupGeneral(QVector<QString> devices,
                      QVector<double> success, QVector<double> ioError[],
                      QVector<double> crashExit, QVector<double> procError[]);

    QString logsPath;
    DeviceQuery *deviceQuery;
    Ui::Stats *ui;
};

#endif // STATS_H
