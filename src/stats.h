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
public slots:
    void slotShowStats();
private:
    void initGraphs();
    void getGeneralData();
    void setupGeneral(QVector<QString> devices,
                      QVector<double> success, QVector<double> ioError[],
                      QVector<double> crashExit, QVector<double> procError[]);

    DeviceQuery *deviceQuery;
    Ui::Stats *ui;
};

#endif // STATS_H
