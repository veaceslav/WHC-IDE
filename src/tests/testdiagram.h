#ifndef TESTDIAGRAM_H
#define TESTDIAGRAM_H
#include <QObject>

class DiagramNode;

class TestDiagram : public QObject
{
    Q_OBJECT

private slots:
    void slotTestAddTask();
    void slotTestAddData();
    void slotTestDeleteItemById();
    void slotTestSearchById();
};

#endif // TESTDIAGRAM_H
