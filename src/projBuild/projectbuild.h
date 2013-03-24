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

#ifndef PROJECTBUILD_H
#define PROJECTBUILD_H

#include "ide.h"
#include <QProcess>
#include <QToolButton>

#ifdef _WIN32
#define OS 1
#elif __linux__
#define OS  2
#endif

class ProjectSettings;
class Ide;
class QDir;
class QTextEdit;
class CommandLine;

class ProjectBuild : public QWidget
{
    Q_OBJECT
public:
    ProjectBuild(QDomDocument *proj, Ide* parent, ProjectSettings* settings,
                 CommandLine* cmd);
    ~ProjectBuild();
    /**
     * @brief start - start building tasks
     */
    void startBuild();

    /**
     * @brief clean - remove directory build with all files from it
     */
    void clean();

private slots:
    /**
     * @brief slotUpdateError - add error output to standardOut(TextEdit)
     */
    void slotUpdateError();
    /**
     * @brief slotUpdateText - add program output to standardOut(TextEdit)
     */
    void slotUpdateText();
    /**
     * @brief slotUpdateExit - increment iterator and calls configureTask()
     */
    void slotNextTask();
    /**
     * @brief slotBuildTask - called after configureTask(cmake) and run "make"
     */
    void slotBuildTask();

private:
    /**
     * @brief configureTask - run cmake, when finished run slotBuildTask();
     */
    void configureTask();

    /**
     * @brief removeDirectory - remove directory recursively
     */
    bool removeDirectory(QDir &aDir);

    /**
     * @brief setEnvironmentVariables -set variables linke path,lib,include
     */
    void setEnvironmentVariables(ProjectSettings* setting);


    /** Display configure/build output **/
    QTextEdit* standardOut;
    Ide* parent;
    QToolButton* closeButton;
    /** Project path **/
    QString path;
    /** current path -shared by configureTask() and buildTask() **/
    QString pathTmp;
    /** nmake path- Windows only **/
    QString nmakePath;
    /** Task iterator **/
    int iter;
    /** Task Paths **/
    QStringList paths;
    /** current process -- needed by slotUpdateError/Text **/
    QProcess* proc;
    /** set process paths - Windows only **/
    QProcessEnvironment env;

    CommandLine* cmd;

};

#endif // PROJECTBUILD_H
