#include "monitor.h"

#include <QStringList>
#include <QDir>

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
    QString logPath = whcFile.remove(whcFile.split("/").last()) + "/log";
    QDir::mkdir(logPath);
    projStatsFile = logPath + "/stats";
    projectStats = new QSettings(projStatsFile , QSettings::IniFormat);
}


void Monitor::slotFinishedExecute()
{
    projectStats->sync();
    delete projectStats;
}
