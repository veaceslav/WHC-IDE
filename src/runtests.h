#ifndef RUNTESTS_H
#define RUNTESTS_H

#include "projBuild/test/testCommandLine.h"
#include "tests/testIde.h"
#include "tests/testdiagram.h"
#include "tests/testdiagramscene.h"

#include <QtTest/QtTest>

void runtests()
{
    int argc = 0;
    char** argv = 0;

    TestCommandLine test;
    QTest::qExec(&test,argc,argv);

    TestIde test1;
    QTest::qExec(&test1,argc,argv);

    TestDiagram test2;
    QTest::qExec(&test2,argc,argv);

    TestDiagramScene test3;
    QTest::qExec(&test3,argc,argv);

}
#endif // RUNTESTS_H
