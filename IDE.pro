# -------------------------------------------------
# Project created by QtCreator 2010-07-27T12:42:27
# -------------------------------------------------
QT += network
QT += xml \
    xmlpatterns \
    widgets \
    testlib

TARGET = IDE
TEMPLATE = app
INCLUDEPATH += src \
            lib \
            include
SOURCES += \
    src/ide.cpp \
    src/highlighter.cpp \
    src/syntaxhighlighter.cpp \
    src/newproject.cpp \
    src/findtool.cpp   \
    src/mditexteditor.cpp \
    src/mdisubwindow.cpp \
    src/main.cpp \
    src/staticmethods.cpp \
    lib/iwf.cpp \
    src/settings/clustersettings.cpp \
    src/model/projecttreemodel.cpp \
    src/model/projecttreeitem.cpp \
    src/cluster/clustercontrol.cpp \
    src/cluster/clusterconnect.cpp \
    src/cluster/addcluster.cpp \
    src/projManagement/addtask.cpp \
    src/projManagement/addfiles.cpp \
    src/projManagement/datagroup.cpp \
    src/projManagement/adddata.cpp \
    src/projManagement/addnewfile.cpp \
    src/projBuild/projectbuild.cpp \
    src/settings/generalsettings.cpp \
    src/diagram/diagramscene.cpp \
    src/diagram/diagramitem.cpp \
    src/diagram/connector.cpp \
    src/diagram/arrow.cpp \
    src/diagram/diagramwindow.cpp \
    src/settings/editorSettings.cpp \
    src/projectExec/sorttask.cpp \
    src/projectExec/execute.cpp \
    src/projBuild/commandline.cpp \
    src/projectExec/devicequery.cpp \
    src/projectExec/oneprocess.cpp \
    src/mdidiagram.cpp \
    src/settings/editorSettingsVar.cpp \
    src/projBuild/test/testCommandLine.cpp \
    src/tests/testide.cpp \
    src/tests/testdiagram.cpp \
    src/tests/testdiagramscene.cpp \
    src/projManagement/overwritefile.cpp \
    src/monitor.cpp

HEADERS += \
    src/ide.h \
    src/highlighter.h \
    src/syntaxhighlighter.h \
    src/settings/projectsettings.h \
    src/mditexteditor.h \
    src/mdisubwindow.h \
    src/newproject.h  \
    src/findtool.h   \
    src/staticmethods.h \
    include/iwf.h \
    src/settings/generalsettings.h \
    src/settings/clustersettings.h \
    src/model/projecttreemodel.h \
    src/model/projecttreeitem.h \
    src/cluster/clustercontrol.h \
    src/cluster/clusterconnect.h \
    src/cluster/addcluster.h \
    src/projManagement/addtask.h \
    src/projManagement/addfiles.h \
    src/projManagement/datagroup.h \
    src/projManagement/adddata.h \
    src/projManagement/addnewfile.h \
    src/projBuild/projectbuild.h \
    src/diagram/diagramwindow.h \
    src/diagram/diagramscene.h \
    src/diagram/diagramitem.h \
    src/diagram/connector.h \
    src/diagram/arrow.h \
    src/settings/editorSettings.h \
    src/diagram/containers.h \
    src/projectExec/sorttask.h \
    src/projectExec/execute.h \
    src/projBuild/commandline.h \
    src/projectExec/devicequery.h \
    src/projectExec/oneprocess.h \
    src/mdidiagram.h \
    src/settings/editorSettingsVar.h \
    src/projBuild/test/testCommandLine.h \
    src/runtests.h \
    src/tests/testIde.h \
    src/tests/testdiagram.h \
    src/tests/testdiagramscene.h \
    src/projManagement/overwritefile.h \
    src/monitor.h

FORMS += \
    src/forms/tabproject.ui \
    src/forms/projectdialog.ui \
    src/forms/intro.ui \
    src/forms/ide.ui \
    src/forms/credits.ui \
    src/forms/clustercontrol.ui \
    src/forms/clusterconnect.ui \
    src/forms/addcluster.ui \
    src/forms/newTask.ui \
    src/forms/addFile.ui \
    src/forms/settingsDialog.ui
RESOURCES += \
    src/resources/icons.qrc \
    src/resources/mainCppTemplate.qrc

OTHER_FILES += \
    src/CMakeLists.txt

unix:!symbian: LIBS += -L$$PWD/lib/x86/ -lOpenCL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/x86/ -lOpenCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/x86/ -lOpenCL

INCLUDEPATH += $$PWD/lib/x86
DEPENDPATH += $$PWD/lib/x86

unix:!symbian: LIBS += -L$$PWD/lib/x64/ -lOpenCL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/x64/ -lOpenCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/x64/ -lOpenCL

INCLUDEPATH += $$PWD/lib/x64
DEPENDPATH += $$PWD/lib/x64
