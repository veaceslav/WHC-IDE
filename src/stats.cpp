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

#include <QFont>
#include <QSettings>
#include <QProcess>

#include "stats.h"
#include "monitor.h"
#include "projectExec/oneprocess.h"
#include "projectExec/devicequery.h"

Stats::Stats(DeviceQuery *devQuery) :
    deviceQuery(devQuery), ui(new Ui::Stats)
{
    logsPath = QString();
    initGraphs();
}

Stats::Stats(DeviceQuery *devQuery, QString whcFile) :
    deviceQuery(devQuery), ui(new Ui::Stats)
{
    logsPath = whcFile.remove(whcFile.split("/").last()) + "log/";
    initGraphs();
}

Stats::~Stats()
{
    this->close();
    delete ui;
}

void Stats::initGraphs()
{
    ui->setupUi(this);
    if(!logsPath.isNull())
    {
        getRunData();
    }
    getGeneralData();

    this->show();
}

void Stats::getRunData()
{
    QSettings runStats(logsPath + "run", QSettings::IniFormat);

    if(!runStats.contains(EXEC_TIME))
        return;

    QMap<int, int> devTime;
    int elapsed  = runStats.value(EXEC_TIME).toInt();
    int procsRan = runStats.value(PROCS_RAN).toInt();
    int procTime = 0;

    for(int i = 0; i < procsRan; i++)
    {
        procTime += runStats.value(QString(PROC_TIME).arg(i)).toInt();
    }



    ui->totalTime->setText(ui->totalTime->text() +
                           QString::number((double)elapsed / 1000) + "s");
}

void Stats::getGeneralData()
{
    QSettings general("WHC", "WHC IDE Monitor");
    int devices = deviceQuery->devicesCount();
    QVector<QString> devNames;
    QVector<double> successVect, ioErrorVect[3], crashExitVect,
            procErrorVect[4];

    for(int i = 0; i <= devices; i++)
    {
        int successes  = 0, crashExits = 0;

        QString success =
                       QString(DEV_RUNS).arg(i).arg(OneProcess::Success).arg(0);
        if(general.contains(success))
            successes = general.value(success).toInt();

        for(int j = OneProcess::Copy; j <= OneProcess::Mkdir; j++)
        {
            int ioErrs = 0;
            QString ioerr =
                       QString(DEV_RUNS).arg(i).arg(OneProcess::IOError).arg(j);
            if(general.contains(ioerr))
                ioErrs = general.value(ioerr).toInt();
            ioErrorVect[j] << ioErrs;
        }

        QString crashExit =
                QString(DEV_RUNS).arg(i).arg(OneProcess::CrashExitError).arg(0);
        if(general.contains(crashExit))
            crashExits = general.value(crashExit).toInt();

        for(int j = QProcess::FailedToStart; j <= QProcess::UnknownError; j++)
        {
            int procErrs = 0;
            QString procErr =
                  QString(DEV_RUNS).arg(i).arg(OneProcess::ProcessError).arg(j);
            if(general.contains(procErr))
                procErrs = general.value(procErr).toInt();
            procErrorVect[j] << procErrs;
        }

        devNames      << deviceQuery->getName(i);
        successVect   << successes;
        crashExitVect << crashExits;
    }
    setupGeneral(devNames, successVect, ioErrorVect, crashExitVect,
                 procErrorVect);
}

void Stats::setupGeneral(QVector<QString> devices,
                         QVector<double> success, QVector<double> ioError[],
                         QVector<double> crashExit, QVector<double> procError[])
{
    /**
     * Preparing bars and colours
     */

    QCPBars *succ = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);

    QCPBars *ioErr[OneProcess::Mkdir + 1];
    for(int i = OneProcess::Copy; i <= OneProcess::Mkdir; i++)
    {
        ioErr[i] = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);
        ui->aggrPlot->addPlottable(ioErr[i]);
    }

    QCPBars *crash = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);

    QCPBars *procErr[QProcess::UnknownError];
    for(int i = QProcess::FailedToStart; i <= QProcess::UnknownError; i++)
    {
        procErr[i] = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);
        ui->aggrPlot->addPlottable(procErr[i]);
    }

    ui->aggrPlot->addPlottable(succ);
    ui->aggrPlot->addPlottable(crash);


    QPen pen;
    pen.setWidthF(1.2);

    succ->setName("Success");
    pen.setColor(QColor(24, 255, 0));
    succ->setPen(pen);
    succ->setBrush(QColor(24, 255, 0, 100));

    {
        QVector<QString> ioErrs;
        ioErrs << "Copy" << "Replace" << "Mkdir";
        for(int i = OneProcess::Copy; i <= OneProcess::Mkdir; i++)
        {
            ioErr[i]->setName("IO Error: " + ioErrs.at(i));
            pen.setColor(QColor(i * 100, 0, 255));
            ioErr[i]->setPen(pen);
            ioErr[i]->setBrush(QColor(i * 100, 0, 255, 100));
        }
    }

    crash->setName("Crash Exit");
    pen.setColor(QColor(0, 0, 0));
    crash->setPen(pen);
    crash->setBrush(QColor(0, 0, 0, 100));


        QVector<QString> procErrs;
        procErrs << "FailedToStart" << "Crashed" << "Timedout" <<
                    "WriteError" << "ReadError" << "UnknownError";
        for(int i = QProcess::FailedToStart; i <= QProcess::UnknownError; i++)
        {
            procErr[i]->setName("Crash Exit: " + procErrs.at(i));
            pen.setColor(QColor(255, i * 50, 0));
            procErr[i]->setPen(pen);
            procErr[i]->setBrush(QColor(255, i * 50, 0, 100));
        }


    ioErr[OneProcess::Copy]->moveAbove(succ);
    for(int i = OneProcess::Copy + 1; i <= OneProcess::Mkdir; i++)
        ioErr[i]->moveAbove(ioErr[i - 1]);
    crash->moveAbove(ioErr[OneProcess::Mkdir]);
    procErr[QProcess::FailedToStart]->moveAbove(crash);
    for(int i = QProcess::FailedToStart + 1; i <= QProcess::UnknownError; i++)
        procErr[i]->moveAbove(procErr[i - 1]);

    /**
     * Preparing x axis
     */

    QVector<double> ticks;
    for(int i = 1; i <= devices.size(); i++)
        ticks << i;
    ui->aggrPlot->xAxis->setAutoTicks(false);
    ui->aggrPlot->xAxis->setAutoTickLabels(false);
    ui->aggrPlot->xAxis->setTickVector(ticks);
    ui->aggrPlot->xAxis->setTickVectorLabels(devices);
    ui->aggrPlot->xAxis->setTickLabelRotation(20);
    ui->aggrPlot->xAxis->setSubTickCount(0);
    ui->aggrPlot->xAxis->setTickLength(0, 4);
    ui->aggrPlot->xAxis->grid()->setVisible(true);
    ui->aggrPlot->xAxis->setRange(0, devices.size() + 1);

    /**
     * Preparing y axis
     */

    double max = 0;
    for(int i = 0; i < devices.size(); i++)
    {
        double current = success[i] + crashExit[i];
        for(int j = OneProcess::Copy; j <= OneProcess::Mkdir; j++)
            current += ioError[j][i];
        for(int j = QProcess::FailedToStart; j <= QProcess::UnknownError; j++)
            current += procError[j][i];
        if(max < current)
            max = current;
    }

    ui->aggrPlot->yAxis->setRange(0, max * 1.2);
    ui->aggrPlot->yAxis->setPadding(5);
    ui->aggrPlot->yAxis->setLabel("Number of runs");
    ui->aggrPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->aggrPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->aggrPlot->yAxis->grid()->setSubGridPen(gridPen);

    /**
     * Setting data
     */

    succ->setData(ticks, success);
    for(int i = OneProcess::Copy; i <= OneProcess::Mkdir; i++)
        ioErr[i]->setData(ticks, ioError[i]);
    crash->setData(ticks,crashExit);

    for(int i = OneProcess::Copy; i <= OneProcess::Mkdir; i++)
        procErr[i]->setData(ticks, procError[i]);

    /**
     * Legend setup
     */

    ui->aggrPlot->legend->setVisible(true);
    ui->aggrPlot->axisRect()->insetLayout()->setInsetAlignment(0,
                                                 Qt::AlignTop|Qt::AlignHCenter);
    ui->aggrPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->aggrPlot->legend->setBorderPen(legendPen);
    QFont legendFont = ui->title->font();
    legendFont.setPointSize(10);
    ui->aggrPlot->legend->setFont(legendFont);
    ui->aggrPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
