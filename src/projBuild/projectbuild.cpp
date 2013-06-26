/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <QTextEdit>
#include <QDomDocument>
#include <QDir>

#include "staticmethods.h"
#include "projectbuild.h"
#include "settings/projectsettings.h"
#include "commandline.h"

ProjectBuild::ProjectBuild(QDomDocument *proj, Ide *parent,
                           ProjectSettings *settings, CommandLine *cmd)
    : QWidget(parent), cmd(cmd)
{

    this->parent = parent;
    proc = NULL;

    QDomNodeList lst = proj->elementsByTagName("task");

    path = parent->whcFile;

    path.remove(path.split("/").last());

    for(int i = 0; i<lst.count(); i++)
     {
        QString tmp = lst.at(i).attributes().namedItem("name").nodeValue();

        paths += path + "src/" + tmp + "/build";

     }
    setEnvironmentVariables(settings);
}

ProjectBuild::~ProjectBuild()
{
}

void ProjectBuild::startBuild()
{
    cmd->showM();
    this->iter = 0;
    pathTmp = paths.at(iter);
    /**
     * Because running this process in syncronous mode will freeze GUI,
     * tasks are executed asyncronous - using signals and slots
     *  when a process ends it emit signal finished()
     * Progression: configureTask() -> buildTask() -> slotNextTask() ->...
     */
    configureTask();
}

void ProjectBuild::clean()
{
    cmd->showM();
    cmd->clearAll();

    for(int i = 0; i<paths.size(); i++)
    {
        cmd->addLine("Cleaning " + paths.at(i), Qt::black);
        QDir toRemove(paths.at(i));

        StaticMethods::removeDirectory(toRemove);

        cmd->addLine("Done!", Qt::black);
    }
}

void ProjectBuild::configureTask()
{
    cmd->addLine("*** Running Cmake ***", Qt::darkGreen);
    QDir dir;
    dir.mkdir(pathTmp);

    /**
     * Copy kernel.cl file to build directory
     */
    QString source(pathTmp);
    source.remove(pathTmp.split("/").last());


    source.append("kernel.cl");
    QString dest = pathTmp + "/kernel.cl";
    QFile file(source);


    file.copy(dest);

    StaticMethods::destroyObj(&proc);
    proc = new QProcess();

    proc->setWorkingDirectory(pathTmp);
    QString program("cmake");
    QStringList arguments;
    /** If Windows  **/
    if(OS == 1)
    {
        proc->setProcessEnvironment(env);
        arguments += "-G";
        arguments += "NMake Makefiles";
    }
    arguments += "..";

    /**
     * Getting cmake output in real time
     */
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(slotUpdateError()));
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotUpdateText()));
    connect(proc, SIGNAL(finished(int)), this, SLOT(slotBuildTask()));
    proc->start(program, arguments);
}

void ProjectBuild::slotBuildTask()
{

    cmd->addLine("*** Building ***", Qt::darkRed);
    delete proc;
    proc = new QProcess();
    proc->setWorkingDirectory(pathTmp);
    QString program;
    QStringList arguments;
    if(OS == 1)
    {
        program.append(nmakePath);
        proc->setProcessEnvironment(env);
    }
    else
    {
        program.append("make");
        /** make -j 4 :) **/
        arguments += "-j";
        arguments += "4";
    }
    /**
     * Getting build output in realtime
     */
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(slotUpdateError()));
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(slotUpdateText()));
    connect(proc, SIGNAL(finished(int)), this, SLOT(slotNextTask()));
    proc->start(program, arguments);
}

void ProjectBuild::slotUpdateError()
{
    QByteArray data = proc->readAllStandardError();
    cmd->addLine(QString(data), Qt::red);
}

void ProjectBuild::slotUpdateText()
{
    QByteArray data = proc->readAllStandardOutput();
    cmd->addLine(QString(data), Qt::black);
}

void ProjectBuild::slotNextTask()
{
    iter++;
    if(iter >= paths.size())
    {
        cmd->addLine("Done!", Qt::black);
        proc->close();
        return;
    }
    pathTmp = paths.at(iter);

    configureTask();

}

void ProjectBuild::setEnvironmentVariables(ProjectSettings *setting)
{
    env = QProcessEnvironment::systemEnvironment();
    QString vsPath  = setting->VStudioPath;
    QString sdkPath = setting->MsSDKPath;

    /** path **/
    env.insert("PATH", env.value("PATH") + ";"  + vsPath
                                         + "\\VC\\BIN");
    env.insert("PATH", env.value("PATH") + ";"  + sdkPath
                                         + "\\Windows\\v7.0A\\bin");
    env.insert("PATH", env.value("PATH") + ";"  + vsPath
                                         + "\\Common7\\IDE\\");
    env.insert("PATH", env.value("PATH") + ";"  + vsPath
                                         + "\\Common7\\Tools");

    /** include **/
    env.insert("INCLUDE", vsPath + "\\VC\\ATLMFC\\INCLUDE");
    env.insert("INCLUDE", env.value("INCLUDE")  + ";" + vsPath
                                                + "\\VC\\INCLUDE");
    env.insert("INCLUDE", env.value("INCLUDE")  + ";" + sdkPath
                                                + "\\Windows\\v7.0A\\include");
    /** lib **/
    env.insert("LIB", sdkPath + "\\Windows\\v7.0A\\lib");
    env.insert("LIB", env.value("LIB")  + ";"   + vsPath
                                        + "\\VC\\lib");
    env.insert("LIB", env.value("LIB")  + ";"+ vsPath
                                        + "\\VC\\ATLMFC\\INCLUDE");
    /** libpath **/
    env.insert("LIBPATH", vsPath + "\\VC\\lib");
    env.insert("LIBPATH", env.value("LIBPATH")  + ";" + vsPath
                                                + "\\VC\\ATLMFC\\INCLUDE");

    nmakePath = vsPath + "\\VC\\BIN\\nmake.exe";
}
