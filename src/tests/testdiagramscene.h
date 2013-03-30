#ifndef TESTDIAGRAMSCENE_H
#define TESTDIAGRAMSCENE_H

#include <QObject>


class TestDiagramScene : public QObject
{
    Q_OBJECT
public:
    explicit TestDiagramScene(QObject *parent = 0);
    
private slots:
    void slotTestRenameItems();
    
};

#endif // TESTDIAGRAMSCENE_H
