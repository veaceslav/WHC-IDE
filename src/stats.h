#ifndef STATS_H
#define STATS_H

#include <QString>
#include "ui_stats.h"
#include "staticmethods.h"

class Stats : public QObject
{
    Q_OBJECT
public:
    Stats();
    Stats(QString whcFile);
public slots:
    void slotShowStats();
private:
    void getGeneralData();
    void setupGeneral(QVector<QString> devices,
                      QVector<double> success, QVector<double> ioError,
                      QVector<double> crashExit, QVector<double> procError);
    Ui::Stats *ui;
};

#endif // STATS_H
