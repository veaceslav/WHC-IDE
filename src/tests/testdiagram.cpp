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

void TestDiagram::slotTestAddData(){
    Ide ide;

    QDomDocument doc;

    DiagramWindow dw(&doc,&ide);

    dw.addData("New Data",10);

    QVERIFY(!dw.data.isEmpty());
}


void TestDiagram::slotTestDeleteItemById(){

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



