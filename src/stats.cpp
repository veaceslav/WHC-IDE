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
        getProjectData();
    }
    getGeneralData();

    this->show();
}

void Stats::getRunData()
{
    QSettings runStats(logsPath + "run", QSettings::IniFormat);

    if(!runStats.contains(EXEC_TIME))
        return;

    QVector<int> devs;
    QVector<int> tasks;

    QMap<int, int> devTime, devProcs;
    QMap<int, int> taskTime, taskProcs;
    QMap<QPair<int, int>, int> devTaskTime;
    QMap<QPair<int, QString>, int> taskFileTime;

    int elapsed  = runStats.value(EXEC_TIME).toInt();
    int procsRan = runStats.value(PROCS_RAN).toInt();
    int procTime = 0;
    int failed   = 0;

    for(int i = 0; i < procsRan; i++)
    {
        int time   = runStats.value(QString(PROC_TIME).arg(i)).toInt();
        int devId  = runStats.value(QString(PROC_DEVID).arg(i)).toInt();
        int status = runStats.value(QString(PROC_EXIT_STATUS).arg(i)).toInt();
        int taskId = runStats.value(QString(PROC_DIAG_ID).arg(i)).toInt();

        QString input = runStats.value(QString(PROC_INPUT).arg(i)).toString();

        taskFileTime[QPair<int, QString>(taskId, input)] = time;

        if(!devs.contains(devId))
            devs.append(devId);
        if(!tasks.contains(taskId))
            tasks.append(taskId);

        if(devTime.contains(devId))
            devTime[devId] += time;
        else
            devTime[devId] = time;

        if(devProcs.contains(devId))
            devProcs[devId]++;
        else
            devProcs[devId] = 1;

        if(taskTime.contains(taskId))
            taskTime[taskId] += time;
        else
            taskTime[taskId] = time;

        if(taskProcs.contains(taskId))
            taskProcs[taskId]++;
        else
            taskProcs[taskId] = 1;

        if(devTaskTime.contains(QPair<int, int>(devId, taskId)))
            devTaskTime[QPair<int, int>(devId, taskId)] += time;
        else
            devTaskTime[QPair<int, int>(devId, taskId)] = time;

        if(status != OneProcess::Success)
            failed++;

        procTime += time;
    }

    setupRun1(elapsed, procTime, procsRan, failed, devs);
    setupRun2(tasks, taskTime, taskProcs, taskFileTime);
    setupRun3(devs, devTime, devProcs);
    setupDevTime(devTaskTime, devs, tasks);
    setupTaskTime(devTaskTime, devs, tasks);
}

void Stats::getProjectData()
{
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

void Stats::setupTaskTime(QMap<QPair<int, int>, int> devTaskTime,
                          QVector<int> devs, QVector<int> tasks)
{
    QCPBars **time = new QCPBars*[devs.size()];
    int colourDiff = 255 / devs.size();

    QPen pen;
    pen.setWidthF(1.2);

    for(int i = 0; i < devs.size(); i++)
    {
        time[i] = new QCPBars(ui->taskTimePlot->xAxis, ui->taskTimePlot->yAxis);
        ui->taskTimePlot->addPlottable(time[i]);

        time[i]->setName(deviceQuery->getName(devs[i]));
        if(time[i]->name().isNull())
            time[i]->setName(QString("Device %1").arg(devs[i]));

        pen.setColor(QColor(0, 255 - colourDiff * i, colourDiff * i));
        time[i]->setPen(pen);
        time[i]->setBrush(QColor(0, 255 - colourDiff * i, colourDiff * i, 100));

        if(i > 0)
            time[i]->moveAbove(time[i - 1]);
    }

    QVector<QString> diagId;
    QVector<double> *times = new QVector<double>[devs.size()];

    for(int i = 0; i < tasks.size(); i++)
    {
        QString idString = QString("Task ID %1").arg(tasks[i]);
        diagId << idString;

        for(int j = 0; j < devs.size(); j++)
            if(devTaskTime.contains(QPair<int, int>(devs[j], tasks[i])))
                times[j] << devTaskTime[QPair<int, int>(devs[j], tasks[i])];
            else
                times[j] << 0;
    }

    /**
     * Preparing x axis
     */

    QVector<double> ticks;
    for(int i = 1; i <= tasks.size(); i++)
        ticks << i;

    ui->taskTimePlot->xAxis->setAutoTicks(false);
    ui->taskTimePlot->xAxis->setAutoTickLabels(false);
    ui->taskTimePlot->xAxis->setTickVector(ticks);
    ui->taskTimePlot->xAxis->setTickVectorLabels(diagId);
    ui->taskTimePlot->xAxis->setTickLabelRotation(20);
    ui->taskTimePlot->xAxis->setSubTickCount(0);
    ui->taskTimePlot->xAxis->setTickLength(0, 4);
    ui->taskTimePlot->xAxis->grid()->setVisible(true);
    ui->taskTimePlot->xAxis->setRange(0, tasks.size() + 1);

    /**
     * Preparing y axis
     */

    double max = 0;
    for(int i = 0; i < tasks.size(); i++)
    {
        double current = 0;
        for(int j = 0; j < devs.size(); j++)
            current += times[j][i];
        if(max < current)
            max = current;
    }

    ui->taskTimePlot->yAxis->setRange(0, max * 1.2);
    ui->taskTimePlot->yAxis->setPadding(5);
    ui->taskTimePlot->yAxis->setLabel("Time (ms)");
    ui->taskTimePlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->taskTimePlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->taskTimePlot->yAxis->grid()->setSubGridPen(gridPen);

    /**
     * Setting data
     */

    for(int i = 0; i < devs.size(); i++)
        time[i]->setData(ticks, times[i]);

    /**
     * Legend setup
     */

    ui->taskTimePlot->legend->setVisible(true);
    ui->taskTimePlot->axisRect()->insetLayout()->setInsetAlignment(0,
                                                 Qt::AlignTop|Qt::AlignHCenter);
    ui->taskTimePlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->taskTimePlot->legend->setBorderPen(legendPen);
    QFont legendFont = ui->title->font();
    legendFont.setPointSize(10);
    ui->taskTimePlot->legend->setFont(legendFont);
    ui->taskTimePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    delete time;
    delete[] times;
}

void Stats::setupDevTime(QMap<QPair<int, int>, int> devTaskTime,
                         QVector<int> devs, QVector<int> tasks)
{
    QCPBars **time = new QCPBars*[tasks.size()];
    int colourDiff = 255 / tasks.size();

    QPen pen;
    pen.setWidthF(1.2);

    for(int i = 0; i < tasks.size(); i++)
    {
        time[i] = new QCPBars(ui->devTimePlot->xAxis, ui->devTimePlot->yAxis);
        ui->devTimePlot->addPlottable(time[i]);

        time[i]->setName(QString("Task ID %1").arg(tasks[i]));

        pen.setColor(QColor(0, 255 - colourDiff * i, colourDiff * i));
        time[i]->setPen(pen);
        time[i]->setBrush(QColor(0, 255 - colourDiff * i, colourDiff * i, 100));

        if(i > 0)
            time[i]->moveAbove(time[i - 1]);
    }

    QVector<QString> devNames;
    QVector<double> *times = new QVector<double>[tasks.size()];

    for(int i = 0; i < devs.size(); i++)
    {
        QString nameString = deviceQuery->getName(devs[i]);
        if(nameString.isNull())
            nameString = QString("Device %1").arg(devs[i]);
        devNames << nameString;

        for(int j = 0; j < tasks.size(); j++)
            if(devTaskTime.contains(QPair<int, int>(devs[i], tasks[j])))
                times[j] << devTaskTime[QPair<int, int>(devs[i], tasks[j])];
            else
                times[j] << 0;
    }

    /**
     * Preparing x axis
     */

    QVector<double> ticks;
    for(int i = 1; i <= devs.size(); i++)
        ticks << i;

    ui->devTimePlot->xAxis->setAutoTicks(false);
    ui->devTimePlot->xAxis->setAutoTickLabels(false);
    ui->devTimePlot->xAxis->setTickVector(ticks);
    ui->devTimePlot->xAxis->setTickVectorLabels(devNames);
    ui->devTimePlot->xAxis->setTickLabelRotation(20);
    ui->devTimePlot->xAxis->setSubTickCount(0);
    ui->devTimePlot->xAxis->setTickLength(0, 4);
    ui->devTimePlot->xAxis->grid()->setVisible(true);
    ui->devTimePlot->xAxis->setRange(0, devs.size() + 1);

    /**
     * Preparing y axis
     */

    double max = 0;
    for(int i = 0; i < devs.size(); i++)
    {
        double current = 0;
        for(int j = 0; j < tasks.size(); j++)
            current += times[j][i];
        if(max < current)
            max = current;
    }

    ui->devTimePlot->yAxis->setRange(0, max * 1.2);
    ui->devTimePlot->yAxis->setPadding(5);
    ui->devTimePlot->yAxis->setLabel("Time (ms)");
    ui->devTimePlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->devTimePlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->devTimePlot->yAxis->grid()->setSubGridPen(gridPen);

    /**
     * Setting data
     */

    for(int i = 0; i < tasks.size(); i++)
        time[i]->setData(ticks, times[i]);

    /**
     * Legend setup
     */

    ui->devTimePlot->legend->setVisible(true);
    ui->devTimePlot->axisRect()->insetLayout()->setInsetAlignment(0,
                                                 Qt::AlignTop|Qt::AlignHCenter);
    ui->devTimePlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->devTimePlot->legend->setBorderPen(legendPen);
    QFont legendFont = ui->title->font();
    legendFont.setPointSize(10);
    ui->devTimePlot->legend->setFont(legendFont);
    ui->devTimePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    delete time;
    delete[] times;
}

void Stats::setupRun1(int elapsed, int procTime, int procsRan, int failed,
                      QVector<int> devs)
{
    int overhead = elapsed - procTime;

    QString line[6];
    line[0] = "Total time: " + QString::number((double)elapsed / 1000) + "s\n";
    line[1] = "Processes time: " + QString::number((double)procTime / 1000) +
              "s (" + QString::number((double)procTime / elapsed * 100) +
              "%)\n";
    line[2] = "Processes ran: " + QString::number(procsRan) + "\n";
    line[3] = "Success rate: " +
              QString::number((double)failed / procsRan + 100) + "% (" +
              QString::number(failed) + " failed)\n";
    line[4] = "IDE overhead: " + QString::number((double)overhead / 1000) +
              "s (" + QString::number((double)overhead / elapsed * 100) +
              "%)\n";
    line[5] = "Number of devices: " + QString::number(devs.size()) + "\n";

    QString textRow1;

    for(int i = 0; i < 6; i++)
        textRow1 += line[i];

    ui->runStats_1->textCursor().insertText(textRow1);
}

void Stats::setupRun2(QVector<int> tasks, QMap<int, int> taskTime,
                      QMap<int, int> taskProcs,
                      QMap<QPair<int, QString>, int> taskFileTime)
{
    QStringList lines;
    for(int i = 0; i < tasks.size(); i++)
    {
        QString name = QString("Task ID %1").arg(tasks[i]);
        QString ran = QString("Processes ran: %1").arg(taskProcs[tasks[i]]);
        QString time = QString("Run time: %1s").
                arg((double)taskTime[tasks[i]] / 1000);
        QString avg = QString("Average time/process: %1ms/proc").
                      arg((double)taskTime[tasks[i]] / taskProcs[tasks[i]]);
        lines << name << ran << time << avg;
        for(QMap<QPair<int, QString>, int>::Iterator j = taskFileTime.begin();
            j != taskFileTime.end(); j++)
            if(j.key().first == tasks[i])
            {
                QString fileTime = QString("Run time for \"%1\": %2ms").
                                   arg(j.key().second).arg(j.value());
                lines << fileTime;
            }
        lines << "\n";
    }
    ui->runStats_2->textCursor().insertText(lines.join("\n"));
}

void Stats::setupRun3(QVector<int> devs, QMap<int, int> devTime,
                      QMap<int, int> devProcs)
{
    QStringList lines;
    for(int i = 0; i < devs.size(); i++)
    {
        QString name = QString("Task %1").arg(devs[i]);
        QString ran = QString("Processes ran: %1").arg(devProcs[devs[i]]);
        QString time = QString("Run time: %1s").
                arg((double)devTime[devs[i]] / 1000);
        QString avg = QString("Average time/process: %1ms/proc\n").
                      arg((double)devTime[devs[i]] / devProcs[devs[i]]);
        lines << name << ran << time << avg;
    }
    ui->runStats_3->textCursor().insertText(lines.join("\n"));
}

void Stats::setupProject(QVector<int> tasks, QVector<double> taskTime[])
{
    QFont legendFont = ui->title->font();
    legendFont.setPointSize(10);
    ui->projPlot->legend->setVisible(true);
    ui->projPlot->legend->setFont(legendFont);
    QPen pen;

    QVector<double> ticks;

    for(int i = 0; i < taskTime[0].size(); i++)
        ticks << i;

    for(int i = 0; i < tasks.size(); i++)
    {
        pen.setColor(QColor(sin(i*1+1.2)*80+80, sin(i*0.3+0)*80+80,
                            sin(i*0.3+1.5)*80+80));
        ui->projPlot->addGraph();
        ui->projPlot->graph()->setPen(pen);
        ui->projPlot->graph()->setName(QString("Task ID %1").arg(tasks[i]));
        ui->projPlot->graph()->setLineStyle(QCPGraph::lsLine);
        ui->projPlot->graph()->setScatterStyle(QCPScatterStyle
                                               (QCPScatterStyle::ssCircle, 5));

        ui->projPlot->graph()->setData(ticks, taskTime[i]);
        ui->projPlot->graph()->rescaleAxes(true);
    }

    // zoom out a bit:
    ui->projPlot->yAxis->scaleRange(1.1, ui->projPlot->yAxis->range().center());
    ui->projPlot->xAxis->scaleRange(1.1, ui->projPlot->xAxis->range().center());
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

    {
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
