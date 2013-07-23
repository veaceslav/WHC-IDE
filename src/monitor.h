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

    int procsRan;

    QTime execTimer;
    QMap<int, QTime *> procTimer;
};

#endif // MONITOR_H
