#include "monitor.h"

#include <QStringList>
#include <QString>
#include <QDir>
#include <QDebug>

/**
 * The total number of runs
 */
#define PROJ_RUNS "projRuns"

/**
 * The total number of processes that ran;
 */
#define PROCS_RAN "procsRan"

/**
 * The device on which the process ran
 */
#define PROC_DEVID "procDevId_Id%1"

/**
 * The time it took to finish the process
 */
#define PROC_TIME "procTime_Id%1"

/**
 * The input file/s for the process
 **/
#define PROC_INPUT "procInput_Id%1"

/**
 * The id of the process in the workflow diagram
 */
#define PROC_DIAG_ID "procDiagId_Id%1"

/**
 * The status of the process (Succress, IOError, Crash exit or Process error)
 */
#define PROC_EXIT_STATUS "procExitStatus_Id%1"

/**
 * Additional info regarding the process exit status
 */
#define PROC_EXIT_INFO "procExitInfo_Id%1"

/**
 * Total execution time
 */
#define EXEC_TIME "execTime"

Monitor::Monitor()
{
    aggregateStats = new QSettings("WHC", "WHC IDE Monitor");
}

Monitor::~Monitor()
{
    delete aggregateStats;
}

void Monitor::slotStartProcess(int devId)
{
    procTimer[devId] = new QTime();
    procTimer[devId]->start();
}

void Monitor::slotStartExecute(QString whcFile)
{
    QString logPath = whcFile.remove(whcFile.split("/").last());
    QDir projectDir(logPath);
    projectDir.mkdir("log");

    projectStats = new QSettings(logPath + "log/stats" , QSettings::IniFormat);
    runStats = new QSettings(logPath + "log/run", QSettings::IniFormat);

    runStats->clear();
    procsRan = 0;

    execTimer.start();
}

void Monitor::slotFinishedProcess(int devId, int taskId, QString *inFiles,
                                  int taskStatus, int moreInfo)
{
    runStats->setValue(QString(PROC_DEVID).arg(procsRan), devId);
    runStats->setValue(QString(PROC_DIAG_ID).arg(procsRan), taskId);
    runStats->setValue(QString(PROC_TIME).arg(procsRan),
                       procTimer[devId]->elapsed());
    runStats->setValue(QString(PROC_INPUT).arg(procsRan), *inFiles);
    runStats->setValue(QString(PROC_EXIT_STATUS).arg(procsRan), taskStatus);
    runStats->setValue(QString(PROC_EXIT_INFO).arg(procsRan), moreInfo);
    procsRan++;

    delete inFiles;
    delete procTimer[devId];
}

void Monitor::slotFinishedExecute()
{
    runStats->setValue(EXEC_TIME, execTimer.elapsed());
    runStats->setValue(PROCS_RAN, procsRan);

    if(!projectStats->contains(PROJ_RUNS))
        projectStats->setValue(PROJ_RUNS, 1);
    else
        projectStats->setValue(PROJ_RUNS,
                               projectStats->value(PROJ_RUNS).toInt() + 1);

    projectStats->sync();
    runStats->sync();

    delete projectStats;
    delete runStats;
}
