#include "testdiagram.h"
#include "ide.h"
#include "diagram/diagramwindow.h"
#include <QtTest/QtTest>

void TestDiagram::slotTestAddTask()
{
    Ide ide;

    QDomDocument doc;

    DiagramWindow dw(&doc,&ide);

    dw.addTask("MyTask",1,1,1);

    QVERIFY(!dw.tasks.isEmpty());

}
