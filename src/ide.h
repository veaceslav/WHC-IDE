/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 * Copyright (C) 2012 by Cosmin Mihai <yonutix.mc@gmail.com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef IDE_H
#define IDE_H
#define DEBUG

#include "ui_ide.h"
#include "ui_credits.h"
#include "mditexteditor.h"
#include "projectExec/execute.h"
#include "settings/editorSettingsVar.h"

class Highlighter;
class MdiTextEditor;

namespace Ui
{
    class Ide;
}

class ProjectBuild;
class ProjectSettings;
class FindToolUI;
class GoTo;
class ProjectTreeModel;
class MdiSubWindow;
class QDomDocument;
class DiagramWindow;
class CommandLine;
class DeviceQuery;
class MdiDiagram;
class EditorSettingsVar;
class Execute;

class Ide : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ide(QWidget *parent = NULL);
    ~Ide();
    FindToolUI *findtool;
    GoTo *gotoTool;
    //is this ever used?
    bool modified;
    QString whcFile;
    ProjectTreeModel *model;
    QHash<QString, Highlighter*> langs;
    EditorSettingsVar *editorSettings;
    bool findText();

    /**
     * @brief startNewProject - open new created project
     *                          used by NewProject object;
     * @param whcFile         - absolute path to .whc file
     */
    void startNewProject(QString whcFiles);

    /**
     * @brief writeXmltoFile - write changes to project tree
     *                         suck as add task, add file
     *                         to .whc project file
     */
    void writeXmltoFile();

    /**
     * @brief addItemsToLayout -add a new element to ide main window
     * @param widget - element to be added
     */
    void addItemsToLayout(QWidget *widget);
    /**
     * @brief removeItems - remove idget from main window
     * @param widget - widget to be removed
     */
    void removeItems(QWidget *widget);

    void addDeviceList(QWidget *widget);

    void deleteDevWindow(QWidget *widget);

    bool mustSaveFlow() const;

    bool mustLog() const;

    inline QMdiArea *getArea() const {return ui->mdiArea;}

    inline DiagramWindow *getDiagram() const {return diagram; }

    inline CommandLine *getCmd() const {return outWindow; }

    virtual void closeEvent(QCloseEvent *);

    inline Ui::Ide *getUi() const {return ui;}

    MdiTextEditor *getCurrentMdiTextEditor() const;

signals:

public slots:
    /**
     * @brief Edit -> Find
     */
    void slotFindString();
    /**
     * @brief Edit -> Replace
     */
    void slotReplaceString();

    /**
     * @brief Tells the IDE that all tasks have finished execution.
     */
    void slotFinishedExec();
    void slotFindButtonTriggered();
    void slotReplaceButtonTriggered();
    void slotReplaceAllButtonTriggered();
    void slotActionGoToLine();
    void slotGoToPressed();
    void slotSavePressed();
    void slotReplaceSpaceLineEnding();


private slots:
    /**
     *  @brief File -> New File or Project
     */
    void slot_New_Project();
    /**
     *  @brief File -> Open File or Project
     */
    void on_actionOpenProject_triggered();

    /**
     * @brief Open Editor Window when double click a source file
     * @param index - element index in ProjectTreeModel
     */
    void on_projectsView_doubleClicked(const QModelIndex &index);
    void on_actionCluster_Control_triggered();
    void on_actionGenerate_IWF_triggered();
    /**
     * @brief Update the cursor position on status bar
     *        shown as Column: x Row: y
     */
    void cursor_position_changed();

    /**
     *  @brief About->Credits
     */
    void slotCredits();

    /**
     * @brief Add close buttons to close
     *        file editor's tabs
     */
    void slotAddCloseButtons();

    /**
     * @brief slotNewTask - add new task to project
     */
    void slotNewTask();

    /**
      * @brief slotAddNewFile - add a new(empty)file
      *                         to a specified task
      */
    void slotAddNewFile();
    /**
     * @brief slotAddFile - add files to tasks,
     *                      warn user if project
     *                      has no tasks.
     */
    void slotAddFiles();

    /**
     * @brief slotAddDataGroup - add a data group to .whc
     *                          project file and create directory
     *                          for each group in data/
     */
    void slotAddDataGroup();

    /**
     * @brief slotAddDataFiles - add data filesto a specific data group
     */
    void slotAddDataFiles();

    /**
     * @brief slotCloseProject -close active project.
     */
    void slotCloseProject();

    /**
     * @brief slotDeleteItem - delete selected item from source directory
     *                         and project tree
     */
    void slotDeleteItem();

    /**
     * @brief slotBuild - create a folder build for each task and run cmake
     *                    and make in that folder
     */
    void slotBuild();

    /**
     * @brief slotClean - remove build folder and it's content from all tasks
     */
    void slotClean();

    /**
     * @brief on_actionOptions_triggered -display Option->Settings Window
     *                                    and manage ide's options
     */
    void on_actionOptions_triggered();

    /**
     * on_actionRun_triggered() - execute tasks based on workflow diagram
     */
    void on_actionRun_triggered();

    /**
     * contextMenu(const Qpoint& poz) - based on item under cursor, Task or Data
     *                                  execute contextTask or contextData
     */
    void contextMenu(const QPoint &poz);

    void on_actionQuery_Devices_triggered();

    void on_actionRename_triggered();

    void on_actionShow_Diagram_triggered();

    void on_actionShow_Log_triggered();

    void on_actionStop_triggered();

    void on_actionForce_Stop_triggered();

    void on_actionRestore_triggered();

private:

    void keyPressEvent(QKeyEvent *event);
    /**
     * @brief createContextMenus() - create Task and Data custom context Menus
     */
    void createContextMenus();

    MdiSubWindow *findMdiChild(const QString &fileName);
    /**
     * @brief disableMenuOptions - disable entries that
     *                             must be unavailable when no project is open
     * @param val - true - disable, false -enable
     */
    void disableMenuOptions(bool val);

    /**
     * @brief disableStopExec - disable/enable stop and force stop menu options.
     * @param val             - true == disable, false == enable
     */
    void disableStopExec(bool val);

    /**
     * @brief startProjectExec - Starts the project execution after the user
     *                           hits run or restore.
     * @param exclusionList    - The tasks that have been finished and will be
     *                           skipped (empty list in case of a fresh run)
     */
    void startProjectExec(QLinkedList<Exclusion> exclusionList);

    bool isProjectModified();

    /**
     * @brief readSettingsfromFile - load WHC IDE's settings
     */
    void readSettingsfromFile();
    Ui::Ide *ui;
    QLabel *status_bar_info;
    QDialog *creditsPage;
    ProjectBuild *build;
    ProjectSettings *settings;

    DiagramWindow *diagram;
    MdiDiagram *diagramSubW;
    bool diagramShow;

    QMenu *contextTask;
    QMenu *contextData;
    QMenu *contextFile;

    CommandLine *outWindow;

    DeviceQuery *devices;

    Execute *exec;

    /**
     * Friend class is used for testing Ide functionality and it need to have
     * access to all private members
     */
    friend class TestIde;
};

#endif // IDE_H
