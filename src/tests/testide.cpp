/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        : 30-03-2013
 * Description : Test class to check ide.cpp functionality
 *
 * Copyright (C) 2013 by Veaceslav Munteanu <slavuttici at gmail dot com>
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

#include <QtTest/QtTest>
#include <QAction>
#include <QDomDocument>

#include "tests/testIde.h"
#include "ide.h"

void TestIde::slotCheckActions()
{
    Ide ide;

    /**
     * Check if project actions are disabled at
     * startup
     */

    QVERIFY(ide.ui->actionAddData->isEnabled() == false);
    QVERIFY(ide.ui->actionAddTask->isEnabled() == false);
    QVERIFY(ide.ui->actionNew_File->isEnabled() == false);
    QVERIFY(ide.ui->actionAddDataGroup->isEnabled() == false);
    QVERIFY(ide.ui->actionAddFile->isEnabled() == false);
    QVERIFY(ide.ui->actionClose_Project->isEnabled() == false);
    QVERIFY(ide.ui->actionRename->isEnabled() == false);
    QVERIFY(ide.ui->actionDelete_Selected->isEnabled() == false);
    QVERIFY(ide.ui->actionBuild_Project->isEnabled() == false);
    QVERIFY(ide.ui->actionClean_All->isEnabled() == false);
    QVERIFY(ide.ui->actionRun->isEnabled() == false);


   ide.disableMenuOptions(false);

   QVERIFY(ide.ui->actionAddData->isEnabled() == true);
   QVERIFY(ide.ui->actionAddTask->isEnabled() == true);
   QVERIFY(ide.ui->actionNew_File->isEnabled() == true);
   QVERIFY(ide.ui->actionAddDataGroup->isEnabled() == true);
   QVERIFY(ide.ui->actionAddFile->isEnabled() == true);
   QVERIFY(ide.ui->actionClose_Project->isEnabled() == true);
   QVERIFY(ide.ui->actionRename->isEnabled() == true);
   QVERIFY(ide.ui->actionDelete_Selected->isEnabled() == true);
   QVERIFY(ide.ui->actionBuild_Project->isEnabled() == true);
   QVERIFY(ide.ui->actionClean_All->isEnabled() == true);
   QVERIFY(ide.ui->actionRun->isEnabled() == true);

}

void TestIde::slotCheckContextMenu()
{
    Ide ide;

    /**
     * Check if context menus were successfully created
     */
    QVERIFY(ide.contextTask != NULL);

    QVERIFY(ide.contextData != NULL);

    QVERIFY(ide.contextFile != NULL);
}
