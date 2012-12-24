#pragma once
#include <QObject>

class TestCommandLine : public QObject
{
  Q_OBJECT
private slots:
  
  void testAdd();

  void testDebugOffAdd();

  void testDebugOnAdd();

};
