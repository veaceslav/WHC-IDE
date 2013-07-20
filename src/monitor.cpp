#include "monitor.h"

Monitor::Monitor()
{
    aggregateStats = new QSettings("WHC", "WHC IDE Monitor");
}

Monitor::~Monitor()
{
    delete aggregateStats;
}


void Monitor::slotStartExecute(QString projParh)
{
    projectStats = new QSettings(projParh, QSettings::IniFormat);
}


void Monitor::slotFinishedExecute()
{
    delete projectStats;
}
