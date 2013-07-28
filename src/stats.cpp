#include <QFont>
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

void Stats::setupGeneral(QVector<QString> devices,
                         QVector<double> success, QVector<double> ioError,
                         QVector<double> crashExit, QVector<double> procError)
{
    /**
     * Preparing bars and colours
     */

    QCPBars *succ = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);
    QCPBars *ioErr = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);
    QCPBars *crash = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);
    QCPBars *procErr = new QCPBars(ui->aggrPlot->xAxis, ui->aggrPlot->yAxis);
    ui->aggrPlot->addPlottable(succ);
    ui->aggrPlot->addPlottable(ioErr);
    ui->aggrPlot->addPlottable(crash);
    ui->aggrPlot->addPlottable(procErr);

    QPen pen;
    pen.setWidthF(1.2);

    succ->setName("Success");
    pen.setColor(QColor(255, 131, 0));
    succ->setPen(pen);
    succ->setBrush(QColor(255, 131, 0, 50));

    ioErr->setName("IO Error");
    pen.setColor(QColor(1, 92, 191));
    ioErr->setPen(pen);
    ioErr->setBrush(QColor(1, 92, 191, 50));

    crash->setName("Crash Exit");
    pen.setColor(QColor(150, 222, 0));
    crash->setPen(pen);
    crash->setBrush(QColor(150, 222, 0, 70));

    procErr->setName("Crash Exit");
    pen.setColor(QColor(1, 22, 100));
    procErr->setPen(pen);
    procErr->setBrush(QColor(1, 22, 100, 70));

    ioErr->moveAbove(succ);
    crash->moveAbove(ioErr);
    procErr->moveAbove(crash);

    /**
     * Preparing x axis
     */

    QVector<double> ticks;
    for(int i = 0; i < devices.size(); i++)
        ticks << i;
    ui->aggrPlot->xAxis->setAutoTicks(false);
    ui->aggrPlot->xAxis->setAutoTickLabels(false);
    ui->aggrPlot->xAxis->setTickVector(ticks);
    ui->aggrPlot->xAxis->setTickVectorLabels(devices);
    ui->aggrPlot->xAxis->setTickLabelRotation(60);
    ui->aggrPlot->xAxis->setSubTickCount(0);
    ui->aggrPlot->xAxis->setTickLength(0, 4);
    ui->aggrPlot->xAxis->grid()->setVisible(true);
    ui->aggrPlot->xAxis->setRange(0, devices.size() + 1);

    /**
     * Preparing y axis
     */

    double max = 0;
    for(int i = 0; i < success.size(); i++)
        if(max < success[i])
            max = success[i];
    for(int i = 0; i < ioError.size(); i++)
        if(max < ioError[i])
            max = ioError[i];
    for(int i = 0; i < crashExit.size(); i++)
        if(max < crashExit[i])
            max = crashExit[i];
    for(int i = 0; i < procError.size(); i++)
        if(max < procError[i])
            max = procError[i];
    ui->aggrPlot->yAxis->setRange(0, max + 1);
    ui->aggrPlot->yAxis->setPadding(5);
    ui->aggrPlot->yAxis->setLabel("Number of runs");
    ui->aggrPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->aggrPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->aggrPlot->yAxis->grid()->setSubGridPen(gridPen);

    /**
     * Setting data
     */

    succ->setData(ticks, success);
    ioErr->setData(ticks, ioError);
    crash->setData(ticks,crashExit);
    procErr->setData(ticks, procError);

    /**
     * Legend setup
     */

    ui->aggrPlot->legend->setVisible(true);
    ui->aggrPlot->axisRect()->insetLayout()->setInsetAlignment(0,
                                                 Qt::AlignTop|Qt::AlignHCenter);
    ui->aggrPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->aggrPlot->legend->setBorderPen(legendPen);
    QFont legendFont = ui->title->font();
    legendFont.setPointSize(10);
    ui->aggrPlot->legend->setFont(legendFont);
    ui->aggrPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
