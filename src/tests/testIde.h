#pragma once
#include <QObject>

class TestIde : public QObject
{
  Q_OBJECT

private slots:

    void slotCheckActions();

    void slotCheckContextMenu();

};
