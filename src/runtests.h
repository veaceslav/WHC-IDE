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

    /**
     * @brief test- Test projBuild's command line
     */
    TestCommandLine test;
    QTest::qExec(&test,argc,argv);

    /**
     * @brief test1- Test Ide main class
     */
    TestIde test1;
    QTest::qExec(&test1,argc,argv);

    /**
     * @brief test2 -Test diagram window
     */
    TestDiagram test2;
    QTest::qExec(&test2,argc,argv);

    /**
     * @brief test3 - Test Diagram Scene
     */
    TestDiagramScene test3;
    QTest::qExec(&test3,argc,argv);

}
#endif // RUNTESTS_H
