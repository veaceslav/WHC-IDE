/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
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

#include "testdiagramscene.h"
#include "../diagram/diagramwindow.h"
#include "../diagram/diagramscene.h"
#include "../diagram/diagramitem.h"
#include "../diagram/connector.h"
#include "../diagram/containers.h"
#include <QtTest/QtTest>

TestDiagramScene::TestDiagramScene(QObject *parent) :
    QObject(parent)
{
}

void TestDiagramScene::slotTestRenameItems(){

    Ide ide;

    QDomDocument docXml;

    QMenu menu;

    DiagramWindow dw(&docXml,&ide);

    DiagramScene ds(&menu,&docXml,&dw);

    QDomNode domNode;

    QString oldName("oldName");
    QString newName("newName");

    DiagramItem di(ds.myItemType,&menu,oldName,domNode,1,1,0,0);

    ds.diagItems.append(&di);
    ds.renameItems(oldName,newName);

    QVERIFY(ds.diagItems.at(0)->Name() == newName);
}

void TestDiagramScene::slotTestGetItemById()
{

    Ide ide;

    QDomDocument docXml;

    QMenu menu;

    DiagramWindow dw(&docXml,&ide);

    DiagramScene ds(&menu,&docXml,&dw);

    QDomNode domNode;
    QString name("itemName");

    int id = 10;
    DiagramItem di(ds.myItemType,&menu,name,domNode,id,1,0,0);

    ds.diagItems.append(&di);

    DiagramItem* test = ds.getItemById(id);

    QVERIFY(test->id() == id);

    test = ds.getItemById(id + 1);

    QVERIFY(test == 0);

    delete test;

}

void TestDiagramScene::slotTestDeleteItemsById()
{
    Ide ide;

    QDomDocument docXml;

    QMenu menu;

    DiagramWindow dw(&docXml,&ide);

    DiagramScene ds(&menu,&docXml,&dw);

    QDomNode domNode;

    QString name("itemName");

    int id = 1;

    DiagramItem di(ds.myItemType,&menu,name,domNode,id,1,0,0);

    ds.diagItems.append(&di);

    ds.diagItems.append(&di);

    ds.deleteItemsById(id,ds.myItemType);

    QVERIFY(ds.diagItems.contains(&di) == true);
}

void TestDiagramScene::slotTestRemoveFromList()
{

    Ide ide;

    QDomDocument docXml;

    QMenu menu;

    DiagramWindow dw(&docXml,&ide);

    DiagramScene ds(&menu,&docXml,&dw);

    QDomNode domNode;

    QString name("itemName");

    int id = 1;

    DiagramItem di(ds.myItemType,&menu,name,domNode,id,1,0,0);

    ds.diagItems.append(&di);

    ds.removeFromList(&di);

    QVERIFY(ds.diagItems.size() == 0);
}

void TestDiagramScene::slotTestLoadDiagram(){
/*
    Ide ide;

    QMenu menu;

    QDomDocument docXml;

    ide.projectXml = &docXml;


    DiagramWindow dw(&docXml,&ide);

    QDomElement dummyElement = docXml.createElement("dummyElement");

    docXml.appendChild(dummyElement);

    QDomElement diagramElement = docXml.createElement("diagram");

    docXml.appendChild(diagramElement);

    QDomNodeList list = docXml.elementsByTagName("diagram");

    QVERIFY(!list.isEmpty());

    DiagramNode node;

    node.id = 1;

    node.name = "newNode";

    QPointF point(1.0,2.0);

    DiagramScene diagScene(&menu, &docXml, &dw);

    diagScene.createItemXml(node,point);

    diagScene.loadDiagram();
*/
}

