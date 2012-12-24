#include <QtTest/QtTest>
#include <QPlainTextEdit>
#include <QDebug>

#include "testCommandLine.h"
#include "../commandline.h"

void TestCommandLine::testAdd()
{
    //QString str = "Hello";
    CommandLine cmd;

    cmd.addLine("Hello",Qt::darkCyan);

    QVERIFY(cmd.window->toPlainText() == "Hello");

}

void TestCommandLine::testDebugOffAdd()
{
    CommandLine cmd;

    cmd.addDebugLine("Debug output");

    QVERIFY(cmd.window->toPlainText().isEmpty());
}

void TestCommandLine::testDebugOnAdd()
{
    CommandLine cmd;

    cmd.debugMode = true;

    cmd.addDebugLine("Debug Output");

    QVERIFY(cmd.window->toPlainText() == "Debug Output");
}
