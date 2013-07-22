#include "monitor.h"

#include <QStringList>
#include <QDir>
#include <QDebug>

Monitor::Monitor()
{
    aggregateStats = new QSettings("WHC", "WHC IDE Monitor");
}

Monitor::~Monitor()
{
    delete aggregateStats;
}


void Monitor::slotStartExecute(QString whcFile)
{
    QString logPath = whcFile.remove(whcFile.split("/").last());
    QDir projectDir(logPath);
    projectDir.mkdir("log");
    projStatsFile = logPath + "/log/stats";
    projectStats = new QSettings(projStatsFile , QSettings::IniFormat);

    qDebug()<<projStatsFile;
    execTimer.start();
}


void Monitor::slotFinishedExecute()
{
    qDebug()<<execTimer.elapsed();

    projectStats->sync();
    delete projectStats;
}
