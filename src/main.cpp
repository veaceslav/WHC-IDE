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

#include <QApplication>
#include "ide.h"
#include <QtTest/QtTest>
#include "runtests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);

    runtests();
    QCoreApplication::setOrganizationName("whc");
    QCoreApplication::setApplicationName("ide");

    Ide ide;  // create main IDE
    ide.showMaximized();

    return a.exec();
}
