#include "stats.h"

Stats::Stats()
{
}

Stats::Stats(QString whcFile)
{
}

void Stats::slotShowStats()
{
}

void Stats::setupGeneral()
{
    QVector<QString> labels;
    labels << "Success" << "IO Error" << "Crash Exit Error" << "Process Error";
}
