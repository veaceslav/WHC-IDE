#include <QFont>
#include <QSettings>
#include <QProcess>

#include "stats.h"
#include "monitor.h"
#include "projectExec/oneprocess.h"
#include "projectExec/devicequery.h"

Stats::Stats(DeviceQuery *devQuery) :
    deviceQuery(devQuery), ui(new Ui::Stats)
{
    initGraphs();
}

Stats::Stats(DeviceQuery *devQuery, QString whcFile) :
    deviceQuery(devQuery), ui(new Ui::Stats)
{
    initGraphs();
}

Stats::~Stats()
{
    delete ui;
}

void Stats::slotShowStats()
{
}

void Stats::initGraphs()
{
    ui->setupUi(this);
    getGeneralData();

    this->show();
}

void Stats::getGeneralData()
{
    QSettings general("WHC", "WHC IDE Monitor");
    int devices = deviceQuery->devicesCount();
    QVector<QString> devNames;
    QVector<double> successVect, ioErrorVect, crashExitVect, procErrorVect;

    for(int i = 0; i <= devices; i++)
    {
        int successes  = 0,
            ioErrs     = 0,
            crashExits = 0,
            procErrs   = 0;

        QString success =
                       QString(DEV_RUNS).arg(i).arg(OneProcess::Success).arg(0);
        if(general.contains(success))
            successes = general.value(success).toInt();

        for(int j = OneProcess::Copy; j < OneProcess::Mkdir; j++)
        {
            QString ioerr =
                       QString(DEV_RUNS).arg(i).arg(OneProcess::IOError).arg(j);
            if(general.contains(ioerr))
                ioErrs += general.value(ioerr).toInt();
        }

        QString crashExit =
                QString(DEV_RUNS).arg(i).arg(OneProcess::CrashExitError).arg(0);
        if(general.contains(crashExit))
            crashExits = general.value(crashExit).toInt();

        for(int j = QProcess::FailedToStart; j <= QProcess::UnknownError; j++)
        {
            QString procErr =
                  QString(DEV_RUNS).arg(i).arg(OneProcess::ProcessError).arg(j);
            if(general.contains(procErr))
                procErrs += general.value(procErr).toInt();
        }

        devNames      << deviceQuery->getName(i);
        successVect   << successes;
        ioErrorVect   << ioErrs;
        crashExitVect << crashExits;
        procErrorVect << procErrs;
    }
    setupGeneral(devNames, successVect, ioErrorVect, crashExitVect,
                 procErrorVect);
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
    for(int i = 1; i <= devices.size(); i++)
        ticks << i;
    ui->aggrPlot->xAxis->setAutoTicks(false);
    ui->aggrPlot->xAxis->setAutoTickLabels(false);
    ui->aggrPlot->xAxis->setTickVector(ticks);
    ui->aggrPlot->xAxis->setTickVectorLabels(devices);
    ui->aggrPlot->xAxis->setTickLabelRotation(20);
    ui->aggrPlot->xAxis->setSubTickCount(0);
    ui->aggrPlot->xAxis->setTickLength(0, 4);
    ui->aggrPlot->xAxis->grid()->setVisible(true);
    ui->aggrPlot->xAxis->setRange(0, devices.size() + 1);

    /**
     * Preparing y axis
     */

    double max = 0;
    for(int i = 0; i < devices.size(); i++)
    {
        double current = success[i] + ioError[i] + crashExit[i] + procError[i];
        if(max < current)
            max = current;
    }

    ui->aggrPlot->yAxis->setRange(0, max * 1.2);
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
