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
    void slotStartProcess();
    void slotStartExecute(QString whcFile);

    void slotFinishedProcess();
    void slotFinishedExecute();

private:
    QSettings *aggregateStats;
    QSettings *projectStats;
    QString projStatsFile;

    QTime execTimer;
    QMap<int, QTime> procTimer;
};

#endif // MONITOR_H
