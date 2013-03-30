#include "testdiagramscene.h"
#include "../diagram/diagramwindow.h"
#include "../diagram/diagramscene.h"
#include "../diagram/diagramitem.h"
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
