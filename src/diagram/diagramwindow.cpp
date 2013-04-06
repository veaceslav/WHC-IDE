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

#include <QtGui>
#include <QLabel>
#include <QDomDocument>
#include <QDebug>

#include "diagramwindow.h"
#include "containers.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "arrow.h"


DiagramWindow::DiagramWindow(QDomDocument* proj,Ide* parent):parent(parent)
{

    createActions();
    createMenus();

    currentTask = -1;
    currentData = -1;

    scene = new DiagramScene(itemMenu,proj, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));

    layout = new QVBoxLayout;
    createToolbars();
    loadData(proj);

    layout->addWidget(editToolBar);

    view = new QGraphicsView(scene);
    layout->addWidget(view);

    this->setLayout(layout);

    setWindowTitle(tr("Workflow Diagram"));

    scene->loadDiagram();
}

DiagramWindow::~DiagramWindow()
{
    delete itemMenu;
}

bool DiagramWindow::itemListEmpty(int id)
{
    if(id == 0)
        return tasks.isEmpty();
    else
        return data.isEmpty();
}

DiagramNode DiagramWindow::searchById(int type, int id)
{
    if(type == 0) // Task
    {
        for(int i=0;i<tasks.count();i++)
            if(tasks.at(i).id == id)
                return tasks.at(i);
    }
    else
    {
        for(int i=0;i<data.count();i++)
            if(data.at(i).id == id)
                return data.at(i);
    }
    /**
     * No way that task doesn't exist, it means
     * a malformed xml file
     */
    qDebug() << "DiagramWindow::searchById: no such node, malformed .whc file"
             << " or incorrect usage of search";
    return DiagramNode();
}

DiagramNode DiagramWindow::returnCurrentItem(int id)
{
    if(id == 0)
    {
        if(!tasks.isEmpty())
        return tasks.at(currentTask);
    }
    else
    {
        if(!data.isEmpty())
        return data.at(currentData);
    }
    DiagramNode tmp;
    return tmp;
}

void DiagramWindow::addTask(QString name, int in, int out, int id)
{
    DiagramNode nod;

    nod.name = name;
    nod.inputs= in;
    nod.outputs = out;
    nod.id = id;

    tasks.append(nod);

    QAction* ac = new QAction(name,taskMenu);
    ac->setData(tasks.size()-1);
    /** First element added, set button text **/
    if(tasks.size() == 1)
    {
        buttonGroup->button(0)->setText(name);
        currentTask = 0;
    }
    taskMenu->addAction(ac);
}

void DiagramWindow::addData(QString name, int id)
{
    DiagramNode nod;

    nod.name = name;
    nod.inputs = 1;
    nod.outputs = 1;
    nod.id = id;

    data.append(nod);

    QAction* ac = new QAction(name,dataMenu);
    ac->setData(data.size()-1);
    if(data.size() == 1)
    {
        buttonGroup->button(1)->setText(name);
        currentData = 0;
    }
    dataMenu->addAction(ac);
}

void DiagramWindow::updateXml()
{
    parent->writeXmltoFile();
}

QVector<QPair<ExecNode, ExecNode> > DiagramWindow::getExecData()
{
    QVector<QPair<ExecNode,ExecNode> > tmp;

    QList<Arrow*> arr = scene->getArrows();

    tmp.reserve(arr.size()+1);
    for(int i=0;i<arr.size();i++)
    {
        tmp.append(arr.at(i)->getExecData());
    }

    return tmp;
}

void DiagramWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons)
    {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }

        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
}

void DiagramWindow::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems())
    {
        if (item->type() == Arrow::Type)
        {
            scene->removeItem(item);

            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);

            scene->deleteArrow(arrow);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            arrow->removeDomNode();
            updateXml();
            delete item;
            continue;
        }

        if (item->type() == DiagramItem::Type)
        {
            DiagramItem* dItem = qgraphicsitem_cast<DiagramItem *>(item);

            dItem->removeArrows();
            dItem->removeDomElement();
            scene->removeFromList(dItem);
        }
        scene->removeItem(item);
        delete item;
    }
    /**
     * after deleting all items,update .whc project file
     */
    updateXml();
}

void DiagramWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void DiagramWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems)
    {
        if (item->zValue() >= zValue &&
                item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void DiagramWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems)
    {
        if (item->zValue() <= zValue &&
                item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void DiagramWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void DiagramWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void DiagramWindow::slotTaskActionTrigger(QAction* action)
{
    /** Update current task index**/
    currentTask = action->data().toInt();
    buttonGroup->button(0)->setText(action->text());
    buttonGroup->button(0)->setChecked(true);
    buttonGroup->button(1)->setChecked(false);
    scene->setItemType(DiagramItem::Task);
    scene->setMode(DiagramScene::InsertItem);
}

void DiagramWindow::slotDataActionTrigger(QAction* action)
{
    /** Update current data index **/
    currentData = action->data().toInt();
    buttonGroup->button(1)->setText(action->text());
    buttonGroup->button(1)->setChecked(true);
    buttonGroup->button(0)->setChecked(false);
    scene->setItemType(DiagramItem::Data);
    scene->setMode(DiagramScene::InsertItem);
}

void DiagramWindow::deleteItemById(int id, int type)
{
    QList<QAction*> actionList;

    DiagramNode nod = searchById(type,id);

    scene->deleteItemsById(id,type);

    int index;

    if(type == 0)
        index = tasks.indexOf(nod);
    else
        index = data.indexOf(nod);

    if(type == 0)
    {
        actionList = taskMenu->actions();
        Q_FOREACH(QAction* action, actionList)
        {
            if(action->data().toInt() == index)
            {
                taskMenu->removeAction(action);
                tasks.removeAt(index);
                if(!itemListEmpty(0))
                {
                    buttonGroup->button(0)->setText(tasks.first().name);
                    currentTask = 0;
                }
                else
                    buttonGroup->button(0)->setText(QString());
                return;
            }
        }
        return;
    }
    if(type == 1)
    {
        actionList = dataMenu->actions();
        Q_FOREACH(QAction* action, actionList)
        {
            if(action->data().toInt() == index)
            {
                dataMenu->removeAction(action);
                data.removeAt(index);
                if(!itemListEmpty(1))
                {
                    buttonGroup->button(1)->setText(data.first().name);
                    currentData = 0;
                }
                else
                    buttonGroup->button(1)->setText(QString());
                return;
            }
        }
        return;
    }
    qDebug() << "Diagram Window: Unknown option";
}

void DiagramWindow::renameDiagItems(QString oldName, QString newName)
{
    scene->renameItems(oldName, newName);
}

void DiagramWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"),
                                 tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"),
                               tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteItem()));
}

void DiagramWindow::createMenus()
{
    itemMenu = new QMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    taskMenu = new QMenu();
    connect(taskMenu,SIGNAL(triggered(QAction*)),
            this,SLOT(slotTaskActionTrigger(QAction*)));
    dataMenu = new QMenu();
    connect(dataMenu,SIGNAL(triggered(QAction*)),
            this,SLOT(slotDataActionTrigger(QAction*)));

}

void DiagramWindow::loadData(QDomDocument* project)
{
    QDomNode projectNode = project->lastChild();

    QDomNode tasksObj = projectNode.namedItem("tasks");
    QDomNode dataObj  = projectNode.namedItem("data");

    QDomNodeList taskList = tasksObj.childNodes();
    QDomNodeList dataList = dataObj.childNodes();

    for(int i=0; i<taskList.count();i++)
    {
        QDomNamedNodeMap atributes =taskList.at(i).attributes();
        DiagramNode nod;
        nod.name = atributes.namedItem("name").nodeValue();
        nod.id = atributes.namedItem("id").nodeValue().toInt();
        nod.inputs = atributes.namedItem("x").nodeValue().toInt();
        nod.outputs = atributes.namedItem("y").nodeValue().toInt();

        tasks.append(nod);
        QAction* ac = new QAction(nod.name,taskMenu);
        ac->setData(i);
        taskMenu->addAction(ac);
    }

    /**
     * Since button has no text, set button text with first task's name
     * if list is empty button will have no text
     */
    if(!taskList.isEmpty())
    {
        buttonGroup->button(0)->setText(tasks.at(0).name);
        currentTask = 0;
    }

    for(int i=0; i<dataList.count();i++)
    {
        QDomNamedNodeMap atributes =dataList.at(i).attributes();
        DiagramNode nod;
        nod.name = atributes.namedItem("name").nodeValue();
        nod.id = atributes.namedItem("id").nodeValue().toInt();
        /**
         * Data nodes have always 1 input and 1 output
         */
        nod.inputs = 1;
        nod.outputs = 1;

        data.append(nod);
        QAction* ac = new QAction(nod.name,dataMenu);
        ac->setData(i);
        dataMenu->addAction(ac);
    }

    if(!dataList.isEmpty())
    {
        buttonGroup->button(1)->setText(data.at(0).name);
        currentData = 0;
    }
}

void DiagramWindow::createToolbars()
{

    editToolBar = new QToolBar(tr("Edit"),this);
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton,
                                int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    editToolBar->addWidget(pointerButton);
    editToolBar->addWidget(linePointerButton);
    editToolBar->addWidget(sceneScaleCombo);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    editToolBar->addWidget(createCellWidget(DiagramItem::Task));

    editToolBar->addWidget(createCellWidget(DiagramItem::Data));

}

QWidget *DiagramWindow::createCellWidget(DiagramItem::DiagramType type)
{
    QToolButton *button = new QToolButton;
    button->setPopupMode(QToolButton::MenuButtonPopup);
    button->setMenu(createItemMenu(type));
    button->setCheckable(true);
    button->setMinimumWidth(75);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
QMenu *DiagramWindow::createItemMenu(DiagramItem::DiagramType type)
{
    if(type == DiagramItem::Task)
        return taskMenu;
    if(type == DiagramItem::Data)
        return dataMenu;

    qDebug() << "DiagramWindow::createItemMenu: unknown diagramItem type";
    return 0;
}
