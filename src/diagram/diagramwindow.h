/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
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

#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QList>

#include "diagramitem.h"
#include "ide.h"
#include "tests/testdiagram.h"
#include "tests/testdiagramscene.h"

class DiagramScene;

class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QVBoxLayout;
class QDomDocument;
class DiagramNode;
class ExecNode;

class DiagramWindow : public QWidget
{
    Q_OBJECT

public:
    DiagramWindow(QDomDocument* proj,Ide* parent);
    ~DiagramWindow();

    /**
     * @brief itemListEmpty - check if data or task list is empty
     * @param id - 0 if item is task and 1 if item is data
     * @return if task/data list is empty
     */
    bool itemListEmpty(int id);
    /**
     * @brief searchById - search diagram node by id
     * @param type - task/data
     * @param id - tas kor data id
     * @return - DiagramNode with matching id
     */
    DiagramNode searchById(int type, int id);

    /**
     * @brief returnCurrentItem - return current index from comboBox
     * @param id 0 - taskComboBox, 1 - dataComboBox
     * @return Diagram node from current index
     */
    DiagramNode returnCurrentItem(int id);

    /**
     * @brief addTask   - add a task to diagram task list, will be called from
     *                    projectManagemet's addTask
     * @param name      - task name
     * @param in        - number of inputs
     * @param out       - number of outputs
     * @param id        - task id
     */
    void addTask(QString name,int in, int out, int id);
    /**
     * @brief addData   - add data node to diagram data list, will be called from
     *                    projectManagement's addData
     * @param name      - data name
     * @param id        - data id
     */
    void addData(QString name,int id);

    /**
     * @brief updateXml -update .whc project file after adding
     *                   a new diagram element
     */
    void updateXml();

    /**
     * @brief getExecData -query each arrow for Execution data and return it
     *                      in a vector
     * @return            - Vector with info about all connections
     */
    QVector<QPair<ExecNode,ExecNode> > getExecData();


    /**
     * @brief deleteItemById - delete all diagram items that is asociated to a
     *                         task/data node
     * @param id             - task id or data id
     * @param type           - 0 - task 1 - data
     */
    void deleteItemById(int id, int type);


public slots:

    /**
     * @brief renameDiagItems - renames the items in the scene after a rename in
     *                          the project tree
     * @param oldName - the name of the items that have been changed
     * @param newName - the name to replace the old name of the items
     */
    void renameDiagItems(QString oldName, QString newName);


private slots:

    /**
     * @brief buttonGroupClicked - Task/Data button group was clicked
     * @param id - 0 = Task, 1 = Data
     */
    void buttonGroupClicked(int id);

    /**
     * @brief deleteItem -delete selected item from diagram scene
     */
    void deleteItem();

    /**
     * @brief pointerGroupClicked -select between normal pointer and line draw
     * @param id 0 = normal, 1 = draw line(set arrow)
     */
    void pointerGroupClicked(int id);

    /**
     * @brief bringToFront - bring to front selected item
     */
    void bringToFront();

    /**
     * @brief sendToBack - send to back selected item(if 2 items collide)
     */
    void sendToBack();

    /**
     * @brief itemInserted -set pointer to default and uncheck button Task/Data
     */
    void itemInserted(DiagramItem* item);

    /**
     * @brief sceneScaleChanged - update diagram scene scale after scale change
     * @param scale - 50% 75% 100% 125% and 150%
     */
    void sceneScaleChanged(const QString& scale);

    void slotTaskActionTrigger(QAction *action);

    void slotDataActionTrigger(QAction *action);

private:

    /**
     * @brief createActions -creates Actions like delete, add
     */
    void createActions();

    /**
     * @brief createMenus -create context menu for diagram scene
     */
    void createMenus();

    /**
     * @brief loadData -get data about tasks and Data objects
     */
    void loadData(QDomDocument *project);

    /**
     * @brief createToolbars - create diagram's toolbar
     */
    void createToolbars();

    /**
     * @brief createCellWidget -create a widget with one QToolButton
     * @param text - Button name
     * @param type - Button Type(Task/Data)
     * @return QWidget with one QToolButton
     */
    QWidget* createCellWidget(const DiagramItem::DiagramType type);

    QMenu* createItemMenu(DiagramItem::DiagramType type);

    QVBoxLayout *layout;
    Ide* parent;

    DiagramScene * scene;
    QGraphicsView* view;

    QAction *addAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;

    QMenu *itemMenu;
    QMenu *taskMenu;
    QMenu *dataMenu;

    QToolBar *editToolBar;
    QToolBar *pointerToolbar;

    QComboBox *sceneScaleCombo;


    QToolBox * toolBox;

    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QComboBox* taskCombo;
    QComboBox* dataCombo;

    QAction *lineAction;

    QList<DiagramNode> tasks;
    QList<DiagramNode> data;

    int currentTask;
    int currentData;

    friend class TestDiagram;
    friend class TestDiagramScene;
};

#endif // DIAGRAMWINDOW_H
