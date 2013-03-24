#ifndef TESTDIAGRAM_H
#define TESTDIAGRAM_H
#include <QObject>

class TestDiagram : public QObject
{
    Q_OBJECT

private slots:
    void slotTestAddTask();
};

#endif // TESTDIAGRAM_H
