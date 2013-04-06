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

#ifndef ADDTASK_H
#define ADDTASK_H
#include <QDialogButtonBox>
#include <QGroupBox>
#include "ide.h"

/**
  * The maximum file names to be shown in the printf of the main.cpp file.
  */
#define MAX_NAMES_NO 10

class Ide;
class AddTask : public QDialog
{
    Q_OBJECT

public:
    AddTask(QDomDocument* proj,Ide* parent);
    ~AddTask();


private slots:

    void slotAddTask();

private:
    QDomDocument* projectXml;
    Ide* parent;


    /**
     * @brief generateMainFile - generates main.cpp file
     * @param path             - the full path and name of the file
     *                           that will be generated
     * @param inputs           - the number of inputs for the task
     * @return                 - return true if the file was generated, false
     *                           in case of an error while opening the main.cpp
     *                           file.
     */
    bool generateMainFile(QString path, int inputs);
    /**
     *  Gui elements
     */
    QDialogButtonBox *buttonBox;
    QLabel *taskLabel;
    QLineEdit *taskName;
    QLabel *xLabel;
    QLineEdit *xPoz;
};

#endif //ADDTASK_H
