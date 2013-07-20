#ifndef MONITOR_H
#define MONITOR_H

#include <QSettings>
#include <QTime>
#include <QMap>

class Monitor
{
public:
    Monitor();
    ~Monitor();

public slots:
    void slotStartProcess();
    void slotStartExecute(QString projPath);

    void slotFinishedProcess();
    void slotFinishedExecute();

private:
    QSettings *aggregateStats;
    QSettings *projectStats;

    QTime execTimer;
    QMap<int, QTime> procTimer;
};

#endif // MONITOR_H
