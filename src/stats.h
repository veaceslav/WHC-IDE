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
    void setupGeneral();
    Ui::Stats *ui;
};

#endif // STATS_H
