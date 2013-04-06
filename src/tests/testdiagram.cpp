/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 * Copyright (C) 2013 by Andrei - Marius Dincu <andrei dot dincu at gmail dot com>
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

#include "testdiagram.h"
#include "ide.h"
#include "diagram/diagramwindow.h"
#include "../diagram/containers.h"
#include <QtTest/QtTest>


void TestDiagram::slotTestAddTask()
{
    Ide ide;

    QDomDocument doc;

    DiagramWindow dw(&doc,&ide);

    dw.addTask("MyTask",1,1,1);

    QVERIFY(!dw.tasks.isEmpty());

}

void TestDiagram::slotTestAddData()
{

    Ide ide;

    QDomDocument doc;

    DiagramWindow dw(&doc,&ide);

    dw.addData("New Data",10);

    QVERIFY(!dw.data.isEmpty());
}


void TestDiagram::slotTestDeleteItemById()
{

    Ide ide;

    QDomDocument doc;

    DiagramWindow dw(&doc,&ide);

    // test data element
    int id = 1;
    int type = 1;

    dw.addData("newData",id);

    dw.deleteItemById(id,type);

    QVERIFY(dw.data.isEmpty());

    // test task element
    id = 1;
    type = 0;

    dw.addTask("newTask",1,1,id);
    dw.deleteItemById(id,type);

    QVERIFY(dw.tasks.isEmpty());
}

void TestDiagram::slotTestSearchById()
{

    Ide ide;

    QDomDocument doc;

    DiagramWindow dw(&doc,&ide);

    int id = 1;
    int type = 1;

    dw.addData("newData",id);

    DiagramNode a = dw.searchById(type,id);

    QVERIFY(a.id == id && a.name == "newData");

    a = dw.searchById(type,id + 1);

    QVERIFY(a.id == -1);


    type = 0;
    dw.addTask("newTask",1,1,id);

    a = dw.searchById(type,id);

    QVERIFY(a.id == id && a.name == "newTask");

    a = dw.searchById(type,id + 1);

    QVERIFY(a.id == -1);
}





