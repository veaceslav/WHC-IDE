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

#include "ide.h"

#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSettings>
#include <QString>
#include <QLinkedList>

#include "mdidiagram.h"
#include "diagram/diagramwindow.h"
#include "findtool.h"
#include "newproject.h"
#include "iwf.h"
#include "staticmethods.h"

#include "model/projecttreemodel.h"
#include "cluster/clustercontrol.h"

#include "projManagement/addtask.h"
#include "projManagement/addfiles.h"
#include "projManagement/adddata.h"
#include "projManagement/datagroup.h"
#include "projManagement/addnewfile.h"


#include "projBuild/projectbuild.h"
#include "projBuild/commandline.h"

#include "settings/generalsettings.h"
#include "settings/projectsettings.h"

#include "projectExec/sorttask.h"
#include "projectExec/execute.h"
#include "projectExec/devicequery.h"

Ide::Ide(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Ide){
    findtool = NULL;
    gotoTool = NULL;
    modified = false;
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/icon.png"), QSize(),
                 QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    creditsPage     = NULL;
    build           = NULL;
    exec            = NULL;
    diagram         = NULL;
    diagramSubW     = NULL;
    devices         = NULL;
    model           = NULL;
    outWindow       = NULL;
    editorSettings  = NULL;

    settings = new ProjectSettings();
    readSettingsfromFile();

    ui->setupUi(this);

    this->setAcceptDrops(true);
    ui->centralwidget->setAcceptDrops(true);
    ui->projectsView->setDragEnabled(true);
    ui->projectsView->setAcceptDrops(true);
    ui->projectsView->setDropIndicatorShown(true);
    status_bar_info = new QLabel("none");
    ui->statusbar->addWidget(status_bar_info);
    ui->actionFind->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    ui->actionOpenProject->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    ui->action_Cut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    ui->action_New_File_or_Project->setShortcut(
                QKeySequence(Qt::CTRL + Qt::Key_N));
    ui->action_Copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    ui->action_Paste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    ui->actionGo_to->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    ui->actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(ui->actionGo_to, SIGNAL(triggered()),
            this, SLOT(slotActionGoToLine()));

    connect(ui->actionFind, SIGNAL(triggered()),
            this, SLOT(slotFindString()));

    connect(ui->actionReplace, SIGNAL(triggered()),
            this, SLOT(slotReplaceString()));

    connect(ui->action_New_File_or_Project, SIGNAL(triggered()),
            this, SLOT(slot_New_Project()));

    connect(ui->actionCredits, SIGNAL(triggered()),
            this, SLOT(slotCredits()));
    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(slotAddCloseButtons()));
    connect(ui->actionBuild_Project, SIGNAL(triggered()),
            this, SLOT(slotBuild()));
    connect(ui->actionClean_All, SIGNAL(triggered()), this, SLOT(slotClean()));
    connect(ui->actionSave, SIGNAL(triggered()),
            this, SLOT(slotSavePressed()));
    connect(ui->actionRemove_end_line_spaces, SIGNAL(triggered()),
            this, SLOT(slotReplaceSpaceLineEnding()));

    ui->projectsView->setContextMenuPolicy(Qt::CustomContextMenu);
    createContextMenus();
    connect(ui->projectsView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(contextMenu(QPoint)));

    connect(ui->actionAddTask, SIGNAL(triggered()), this, SLOT(slotNewTask()));
    connect(ui->actionNew_File, SIGNAL(triggered()),
            this, SLOT(slotAddNewFile()));
    connect(ui->actionAddFile, SIGNAL(triggered()),
            this, SLOT(slotAddFiles()));
    connect(ui->actionAddDataGroup, SIGNAL(triggered()),
            this, SLOT(slotAddDataGroup()));
    connect(ui->actionAddData, SIGNAL(triggered()),
            this, SLOT(slotAddDataFiles()));
    connect(ui->actionDelete_Selected, SIGNAL(triggered()), this,
            SLOT(slotDeleteItem()));

    /**
     * A command line window that will display output from Build/Run
     */
    outWindow = new CommandLine(this);

    devices = new DeviceQuery(this);
    /**
     * while no project is open, this options must be disabled
     */
    disableMenuOptions(true);
    disableStopExec(true);
    editorSettings = new EditorSettingsVar(this);

    connect(ui->actionClose_Project, SIGNAL(triggered()),
            this, SLOT(slotCloseProject()));
}

Ide::~Ide()
{

    foreach(Highlighter *h, langs)
    {
        StaticMethods::destroyObj(&h);
    }

    StaticMethods::destroyObj(&settings);
    StaticMethods::destroyObj(&contextTask);
    StaticMethods::destroyObj(&contextFile);
    StaticMethods::destroyObj(&contextData);

    StaticMethods::destroyObj(&creditsPage);
    StaticMethods::destroyObj(&build);
    StaticMethods::destroyObj(&diagram);
    StaticMethods::destroyObj(&diagramSubW);
    StaticMethods::destroyObj(&devices);
    StaticMethods::destroyObj(&exec);

    StaticMethods::destroyObj(&status_bar_info);
    StaticMethods::destroyObj(&editorSettings);
    StaticMethods::destroyObj(&findtool);
    StaticMethods::destroyObj(&gotoTool);
    StaticMethods::destroyObj(&model);
    StaticMethods::destroyObj(&outWindow);
    StaticMethods::destroyObj(&ui);
}

void Ide::slot_New_Project()
{
    NewProject *np = new NewProject(this);
    np->show();
}


void Ide::writeXmltoFile()
{

    QFile file(whcFile);
    file.open(QFile::WriteOnly);
    QByteArray data(model->getProjectXml()->toByteArray());
    QDataStream stream( &file );
    stream.writeRawData(data.data(), data.size());
    file.close();
}

void Ide::slotFindButtonTriggered()
{
    if(findtool->search_solution == NULL)
        QMessageBox::warning(0, tr("Note:"), tr("Something went wrong!\n"));
    if(findtool->search_solution->isChecked())
    {
        if(whcFile.isNull())
            QMessageBox::warning(0, tr("Note:"), tr("No project opened!\n"));
        else
        {
            findtool->showSearchInFilesResults(
                        QFileInfo(whcFile).absolutePath());
            ui->verticalLayout->removeWidget(findtool->search_sol_w);
            ui->verticalLayout->addWidget(findtool->search_sol_w);
        }
    }
    else
    {
        if(!findText())
            QMessageBox::warning(0, tr("Note:"), tr("Nothing found\n"));
    }
}



bool Ide::findText()
{
    MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
    if(!currentDoc)
        return false;

    if(findtool->next_prev_bt->isChecked())
    {
        if(findtool->regex_bt->isChecked())
        {
            QRegExp expression = QRegExp(findtool->searchedText());
            QTextDocument *doc = currentDoc->document();
            QTextCursor founded =  doc->find(expression,
                                             currentDoc->textCursor(),
                                             QTextDocument::FindBackward);
            if(founded == QTextCursor())
            {
                return false;
            }
            else
            {
                currentDoc->find(founded.selectedText(),
                                 QTextDocument::FindBackward);
                return true;
            }
        }
        else
        {
            if(findtool->sensitive_bt->isChecked())
            {
                if(!currentDoc->find(findtool->searchedText(),
                                     QTextDocument::FindBackward |
                                     QTextDocument::FindCaseSensitively))
                {
                    return false;
                }
                return true;
            }
            else
            {
                if(!currentDoc->find(findtool->searchedText(),
                                     QTextDocument::FindBackward))
                {
                    return false;
                }
                return true;
            }
        }
    }
    else
    {
        if(findtool->regex_bt->isChecked())
        {
            QRegExp expression = QRegExp(findtool->searchedText());
            QTextDocument *doc = currentDoc->document();
            QTextCursor founded =  doc->find(expression,
                                             currentDoc->textCursor());
            if(founded == QTextCursor())
            {
                return false;
            }
            else
            {
                currentDoc->find(founded.selectedText());
                return true;
            }
        }
        else
        {
            if(findtool->sensitive_bt->isChecked())
            {
                if(!currentDoc->find(findtool->searchedText(),
                                     QTextDocument::FindCaseSensitively))
                {
                    return false;
                }
                return true;
            }
            else
            {
                if(!currentDoc->find(findtool->searchedText()))
                {
                    return false;
                }
                return true;
            }
        }
    }
}

void Ide::slotReplaceButtonTriggered()
{
    MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
    if(!currentDoc)
        return;
    if(currentDoc->textCursor() == QTextCursor())
    {
        if(findText())
        {
            currentDoc->textCursor().insertText(findtool->rplc->text());
        }
        else
        {
            QMessageBox::warning(0, tr("Note:"), tr("Nothing found\n"));
        }
    }
    else
    {
        currentDoc->textCursor().insertText(findtool->rplc->text());
    }
}

void Ide::slotReplaceAllButtonTriggered()
{
    MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
    if(!currentDoc)
        return;
    QTextCursor cursor = currentDoc->textCursor();
    cursor.setPosition(QTextCursor::Start);
    currentDoc->setTextCursor(cursor);
    while(findText())
    {
        currentDoc->textCursor().insertText(findtool->rplc->text());
    }
}

void Ide::slotActionGoToLine()
{
    if(findtool != NULL)
    {
        ui->verticalLayout->removeWidget(findtool->getFindUI());
        disconnect(SLOT(slotFindButtonTriggered()));
        delete findtool;
        findtool = NULL;
    }
    if(gotoTool == NULL)
    {
        gotoTool = new GoTo();
        ui->verticalLayout->addWidget(gotoTool->getGotoObj());
        gotoTool->getLineEdit()->setFocus();
        connect(gotoTool->getLineEdit(), SIGNAL(returnPressed()),
                this, SLOT(slotGoToPressed()));
    }
    else
    {
        disconnect(this, SLOT(slotGoToPressed()));
        delete gotoTool;
        gotoTool = NULL;
    }
}

void Ide::slotSavePressed()
{
    bool success = true;
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        if (MdiSubWindow *mdiSubWindow = qobject_cast<MdiSubWindow*>(window))
        {
            if(!mdiSubWindow->saveDocument())
            {
                success = false;
            }
            else
            {
                mdiSubWindow->setUnmodified();
            }
        }
    }
    if(!success)
    {
        QMessageBox::warning(0, tr("Warrning:"),
                             tr("One or more files could not be saved.\nPlease check files permissions."));
    }
}

void Ide::slotGoToPressed()
{
    MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
    if(!currentDoc)
        return;
    bool ok;
    int to = gotoTool->getText().toInt(&ok);
    if(ok)
    {
        currentDoc->goTo(to);
    }
    else
    {
        QMessageBox::warning(0,
                             tr("Warning!"),
                             tr("Bad things or nothing inserted.\n"
                                "Check text field!"));
    }
}

void Ide::slotFindString()
{
    if(findtool == NULL)
    {
        findtool = new FindToolUI();
        ui->verticalLayout->addWidget(findtool->find_w);
        connect(findtool->find_bt, SIGNAL(clicked()),
                this, SLOT(slotFindButtonTriggered()));
        connect(findtool->srch, SIGNAL(returnPressed()), this, SLOT(slotFindButtonTriggered()));
        findtool->srch->setFocus();
    }
    else
    {
        if(findtool->replace_w == NULL)
        {
            disconnect(SLOT(slotFindButtonTriggered()));
            ui->verticalLayout->removeWidget(findtool->find_w);
            ui->verticalLayout->removeWidget(findtool->search_sol_w);
            delete findtool;
            findtool = NULL;
        }
        else
        {
            disconnect(SLOT(slotReplaceButtonTriggered()));
            disconnect(SLOT(slotReplaceAllButtonTriggered()));
            ui->verticalLayout->removeWidget(findtool->replace_w);
            findtool->collapseReplace();
            findtool->searchInFilesMainGUI();
        }
    }
}


void Ide::slotReplaceString()
{
    if(findtool == NULL)
    {
        /*If no tool is activated*/
        findtool = new FindToolUI();
        findtool->removeSearchInFileUI();
        findtool->extendReplaceGUI();
        ui->verticalLayout->addWidget(findtool->getFindUI());
        ui->verticalLayout->addWidget(findtool->getReplaceUI());
        connect(findtool->find_bt, SIGNAL(clicked()),
                this, SLOT(slotFindButtonTriggered()));
        connect(findtool->srch, SIGNAL(returnPressed()),
                this, SLOT(slotFindButtonTriggered()));
        connect(findtool->replace_bt, SIGNAL(clicked()),
                this, SLOT(slotReplaceButtonTriggered()));
        connect(findtool->rplc, SIGNAL(returnPressed()),
                this, SLOT(slotReplaceButtonTriggered()));
        connect(findtool->replace_all_bt, SIGNAL(clicked()),
                this, SLOT(slotReplaceAllButtonTriggered()));
        findtool->srch->setFocus();
    }
    else
    {
        if(findtool->getReplaceUI() == NULL)
        {
            findtool->removeSearchInFileUI();
            findtool->extendReplaceGUI();
            ui->verticalLayout->addWidget(findtool->getReplaceUI());
            //QPushButton *b = findtool->replace_bt;
            connect(findtool->replace_bt, SIGNAL(clicked()),
                    this, SLOT(slotReplaceButtonTriggered()));
            connect(findtool->replace_all_bt, SIGNAL(clicked()),
                    this, SLOT(slotReplaceAllButtonTriggered()));
        }
        else
        {
            disconnect(SLOT(slotFindButtonTriggered()));
            disconnect(SLOT(slotReplaceButtonTriggered()));
            disconnect(SLOT(slotReplaceAllButtonTriggered()));
            ui->verticalLayout->removeWidget(findtool->getFindUI());
            ui->verticalLayout->removeWidget(findtool->getReplaceUI());
            delete findtool;
            findtool = NULL;
        }
    }
}

void Ide::slotFinishedExec()
{
    StaticMethods::destroyObj(&exec);
    disableStopExec(true);
    disableMenuOptions(false);
}

void Ide::startNewProject(QString whcFiles)
{
    whcFile = whcFiles;

    ui->mdiArea->closeAllSubWindows();
    model = new ProjectTreeModel(whcFile, this);

    ui->projectsView->setModel(model);
    ui->projectsView->expandToDepth(1);

    /** Init and show Diagram **/
    diagram = new DiagramWindow(model->getProjectXml(), this);

    diagramSubW = new MdiDiagram(diagram,ui->mdiArea,
                              Qt::CustomizeWindowHint |
                              Qt::WindowTitleHint|
                              Qt::WindowMinMaxButtonsHint);

    ui->mdiArea->addSubWindow(diagramSubW);
    diagramShow = true;
    diagramSubW->show();

    connect(model, SIGNAL(updateDiagram(QString, QString)),
            diagram, SLOT(renameDiagItems(QString, QString)));

    /**
     * after staring the project, we must enable this options
     */
    disableMenuOptions(false);
    disableStopExec(true);
}

void Ide::on_actionOpenProject_triggered()
{
    whcFile = QFileDialog::getOpenFileName(this,
                                           tr("Open WHC project"),
                                           "",
                                           tr("WHC Project File (*.whc)"));
    if(whcFile.isNull())
        return;

    startNewProject(whcFile);
}

void Ide::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        if(findtool != NULL)
        {
            ui->verticalLayout->removeWidget(findtool->find_w);
            ui->verticalLayout->removeWidget(findtool->replace_w);
            ui->verticalLayout->removeWidget(findtool->search_sol_w);
            disconnect(SLOT(slotFindButtonTriggered()));
            disconnect(SLOT(slotReplaceButtonTriggered()));
            disconnect(SLOT(slotReplaceAllButtonTriggered()));
            delete findtool;
            findtool = NULL;
        }
        if(gotoTool != NULL)
        {

            delete gotoTool;
            gotoTool = NULL;
        }
    }
}

void Ide::createContextMenus()
{
    contextTask = new QMenu(this);
    contextData = new QMenu(this);
    contextFile = new QMenu(this);

    QIcon icon1(":/images/addTask.png");
    QIcon icon2(":/images/add.png");
    QIcon icon3(":/images/addFiles.png");
    QIcon icon4(":/images/deletefile.png");
    QIcon icon5(":/images/rename.png");

    ui->actionAddTask->setIcon(icon1);
    ui->actionNew_File->setIcon(icon2);
    ui->actionAddFile->setIcon(icon3);
    ui->actionDelete_Selected->setIcon(icon4);
    ui->actionRename->setIcon(icon5);

    ui->actionAddDataGroup->setIcon(icon1);
    ui->actionAddData->setIcon(icon3);

    contextTask->addAction(ui->actionAddTask);
    contextTask->addAction(ui->actionNew_File);
    contextTask->addAction(ui->actionAddFile);
    contextTask->addAction(ui->actionRename);
    contextTask->addAction(ui->actionDelete_Selected);

    contextData->addAction(ui->actionAddDataGroup);
    contextData->addAction(ui->actionAddData);
    contextData->addAction(ui->actionRename);
    contextData->addAction(ui->actionDelete_Selected);

    contextFile->addAction(ui->actionRename);
    contextFile->addAction(ui->actionDelete_Selected);
}

MdiSubWindow *Ide::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        if (MdiSubWindow *mdiSubWindow = qobject_cast<MdiSubWindow*>(window))
            if (mdiSubWindow->currentFile() == canonicalFilePath)
                return (MdiSubWindow*)window;
    }
    return 0;
}


void Ide::on_projectsView_doubleClicked(const QModelIndex &index)
{
    ProjectTreeItem *item = NULL;
    if (index.isValid())
        item = static_cast<ProjectTreeItem*>(index.internalPointer());
    if (!item)
        return;

    QString fileName(index.data(ProjectTreeModel::FileNameRole).toString());
    if (!fileName.isEmpty())
    {
        // create mdi child
        MdiSubWindow *child = findMdiChild(fileName);
        if (child == NULL){
            child = new MdiSubWindow(fileName, this);
            ui->mdiArea->addSubWindow(child);
            child->show();
        }
        else
        {
            ui->mdiArea->setActiveSubWindow((QMdiSubWindow*)child);
        }
        MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
        if(!currentDoc)
            return;
        connect(currentDoc, SIGNAL(cursorPositionChanged()),
                this, SLOT(cursor_position_changed()));
    }
}

void Ide::cursor_position_changed()
{
    MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
    if(!currentDoc)
        return;
    status_bar_info->setText(QString("Column: ") +
                             QString::number(currentDoc->textCursor().
                                             columnNumber(), 10) +
                             QString("  Line: ") +
                             QString::number(currentDoc->textCursor().
                                             blockNumber() + 1, 10));
}

void Ide::on_actionCluster_Control_triggered()
{
    ClusterConnect *clusterConnect = new ClusterConnect();
    clusterConnect->show();
}

//Dead code
void Ide::on_actionGenerate_IWF_triggered()
{
    if(model)
    {
        Iwf iwf(model->file().canonicalFilePath());
    }
    else
    {
        QMessageBox::warning(this, "No open project", "Open a project first.");
    }
}



void Ide::slotCredits()
{

    if(!creditsPage)
        creditsPage = new QDialog(this);

    Ui::Credits uiCred;

    uiCred.setupUi(creditsPage);

    connect(uiCred.pushOk, SIGNAL(clicked()), creditsPage, SLOT(close()));

    creditsPage->show();

}

void Ide::slotAddCloseButtons()
{
    Q_FOREACH (QTabBar *tab, ui->mdiArea->findChildren<QTabBar*>())
    {
       tab->setTabsClosable(true);
       tab->setMovable(true);
    }
}

void Ide::slotNewTask()
{

    AddTask *at = new AddTask(model->getProjectXml(), this);
    at->show();
}

void Ide::slotAddNewFile()
{
    /**
     * If no task is available, return.
     */
    QDomNodeList tasks = model->getProjectXml()->elementsByTagName("task");
    if(tasks.isEmpty())
    {
        QMessageBox msg;
        msg.resize(400,150);
        msg.setText("Project have no tasks.");
        msg.setInformativeText("Please add at least one task before adding files");
        msg.exec();
        return;
    }

    QModelIndex index;
    QModelIndexList lst = ui->projectsView->selectionModel()->selectedIndexes();

    if(lst.size()>0)
        index = lst.at(0);
    AddNewFile *anf = new AddNewFile(model->getProjectXml(), this, index);
    anf->show();
}
void Ide::slotAddFiles()
{
    /**
     * If no task is available, return.
     */
    QDomNodeList tasks = model->getProjectXml()->elementsByTagName("task");
    if(tasks.isEmpty())
    {
        QMessageBox msg;
        msg.resize(400, 150);
        msg.setText("Project have no tasks.");
        msg.setInformativeText("Please add at least one task before adding files");
        msg.exec();
        return;
    }

    QModelIndex index;
    QModelIndexList lst = ui->projectsView->selectionModel()->selectedIndexes();

    if(lst.size() > 0)
        index = lst.at(0);

    AddFiles *af = new AddFiles(model->getProjectXml(), this, index);
    af->show();
}

void Ide::slotAddDataGroup()
{
    DataGroup *gd = new DataGroup(model->getProjectXml(), this);
    gd->show();
}

void Ide::slotAddDataFiles()
{
    /**
     * data files can be added only to a specific data group.
     * If, no data groups available, return.
     */
    QDomNodeList tasks = model->getProjectXml()->elementsByTagName("group");
    if(tasks.isEmpty())
    {
        QMessageBox msg;
        msg.setText("Project have no data groups.");
        msg.setInformativeText("Please add at least one data group before adding files");
        msg.exec();
        return;
    }

    QModelIndex index;
    QModelIndexList lst = ui->projectsView->selectionModel()->selectedIndexes();

    if(!lst.isEmpty())
        index = lst.at(0);

    AddData *ad = new AddData(model->getProjectXml(), this, index);
    ad->show();
}

void Ide::slotCloseProject()
{
    ui->mdiArea->closeAllSubWindows();
    ui->projectsView->setModel(new QStandardItemModel(this));
    StaticMethods::destroyObj(&model);
    StaticMethods::destroyObj(&diagram);
    StaticMethods::destroyObj(&diagramSubW);
    StaticMethods::destroyObj(&exec);

    disableMenuOptions(true);
    disableStopExec(true);
}

void Ide::slotDeleteItem()
{
    QItemSelectionModel *sel = ui->projectsView->selectionModel();

    QModelIndexList lst = sel->selectedIndexes();

    StaticMethods::deleteItem(this, lst);

    writeXmltoFile();

}

void Ide::slotBuild()
{
    if(!build)
        build = new ProjectBuild(model->getProjectXml(), this, settings,
                                                                    outWindow);
    else
    {
        delete build;
        build = new ProjectBuild(model->getProjectXml(), this, settings,
                                                                    outWindow);
    }
    build->startBuild();
}

void Ide::slotClean()
{
    if(!build)
    {
        build = new ProjectBuild(model->getProjectXml(), this, settings,
                                                                    outWindow);
    }
    else
    {
        delete build;
        build = new ProjectBuild(model->getProjectXml(), this, settings,
                                                                    outWindow);
    }
    build->clean();
}

void Ide::disableMenuOptions(bool val)
{
    ui->actionAddData->setDisabled(val);
    ui->actionAddTask->setDisabled(val);
    ui->actionNew_File->setDisabled(val);
    ui->actionAddDataGroup->setDisabled(val);
    ui->actionAddFile->setDisabled(val);
    ui->actionClose_Project->setDisabled(val);
    ui->actionRename->setDisabled(val);
    ui->actionDelete_Selected->setDisabled(val);

    ui->actionBuild_Project->setDisabled(val);
    ui->actionClean_All->setDisabled(val);
    ui->actionRun->setDisabled(val);
    ui->actionShow_Diagram->setDisabled(val);
    ui->actionRestore->setDisabled(val);
}

void Ide::disableStopExec(bool val)
{
    ui->actionStop->setDisabled(val);
    ui->actionForce_Stop->setDisabled(val);
}

void Ide::addItemsToLayout(QWidget *widget)
{
    ui->verticalLayout->addWidget(widget);
}

void Ide::removeItems(QWidget *widget)
{
    ui->verticalLayout->removeWidget(widget);
}

void Ide::addDeviceList(QWidget *widget)
{
    ui->verticalLayout_2->addWidget(widget);
}

void Ide::deleteDevWindow(QWidget *widget)
{
    ui->verticalLayout_2->removeWidget(widget);
}

bool Ide::mustSaveFlow() const
{
    return settings->saveFlow;
}

bool Ide::isProjectModified()
{
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList())
    {
        if (MdiSubWindow *mdiSubWindow = qobject_cast<MdiSubWindow*>(window))
        {
            if(mdiSubWindow->isModified())
                return true;
        }
    }
    return false;
}

void Ide::closeEvent(QCloseEvent *event)
{
    if(isProjectModified())
    {
        QMessageBox::StandardButton answ = QMessageBox::question(this, tr("WHC"),
                     tr("The project has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(answ == QMessageBox::Save)
        {
            slotSavePressed();
            event->accept();
        }
        if(answ == QMessageBox::Discard)
        {
            event->accept();
        }
        if(answ == QMessageBox::Cancel)
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}

void Ide::on_actionOptions_triggered()
{
    GeneralSettings *gs = new GeneralSettings(this, settings);
    gs->show();
}

void Ide::readSettingsfromFile()
{
    QSettings sets("WHC", "WHC IDE");

    if(sets.allKeys().count() == 0)
        return;

    settings->clPath        = sets.value("clPath").toString();
    settings->libclPath     = sets.value("libclPath").toString();
    settings->MsSDKPath     = sets.value("MsSDKPath").toString();
    settings->VStudioPath   = sets.value("VStudioPath").toString();
    settings->saveFlow      = sets.value("saveFlow").toBool();
}

void Ide::startProjectExec(QIODevice::OpenMode fileMode,
                           QLinkedList<Exclusion> exclusionList)
{
    QVector<int> devs = devices->getSelection();
    if(devs.isEmpty())
    {
        outWindow->show();
        outWindow->addLine("No devices selected or no devices available, stop",
                           Qt::red);
        return;
    }

    QVector<QPair<ExecNode,ExecNode> > tmp = diagram->getExecData();

    SortTasks *srt = new SortTasks(this, tmp);

    if(srt->containsCycle())
    {
        outWindow->addLine("The Workflow Diagram contains a cyclic graph.",
                           Qt::red);
    }
    else
    {
        disableStopExec(false);
        disableMenuOptions(true);

        StaticMethods::destroyObj(&exec);
        exec = new Execute(whcFile, srt->getExecutionOrder(), devs, this,
                           outWindow, fileMode, exclusionList);
    }
}

void Ide::on_actionRun_triggered()
{
    QLinkedList<Exclusion> ex;
    startProjectExec(QIODevice::WriteOnly, ex);
}

void Ide::contextMenu(const QPoint &poz)
{
    QModelIndex index = ui->projectsView->indexAt(poz);

    if(!index.isValid())
        return;

    QString type = index.data(ProjectTreeModel::IdRole).toString();

    if(type.isEmpty())
        return;

    if(type == "tasks" || type == "task")
    {
        contextTask->exec(ui->projectsView->mapToGlobal(poz));
        return;
    }
    if(type == "data" || type == "group")
    {
        contextData->exec(ui->projectsView->mapToGlobal(poz));
        return;
    }
    if(type == "file")
    {
        contextFile->exec(ui->projectsView->mapToGlobal(poz));
    }
}

void Ide::on_actionQuery_Devices_triggered()
{
    devices->showDev();
}

void Ide::on_actionRename_triggered()
{
    QModelIndexList lst = ui->projectsView->selectionModel()->selectedIndexes();
    if(!lst.isEmpty())
    {
        QModelIndex index = lst.at(0);
        ui->projectsView->edit(index);
    }
}

void Ide::on_actionShow_Log_triggered()
{
    outWindow->showM();
}

void Ide::on_actionShow_Diagram_triggered()
{
    if(diagramSubW->isVisible())
        return;
    diagramSubW->addtoMdi();
}

void Ide::slotReplaceSpaceLineEnding()
{
    MdiTextEditor *currentDoc = getCurrentMdiTextEditor();
    if(!currentDoc)
        return;
    currentDoc->replaceSpacesLineEnd();
}

MdiTextEditor *Ide::getCurrentMdiTextEditor() const
{
    QMdiSubWindow *win = ui->mdiArea->currentSubWindow();
    if(win == NULL)
        return NULL;
    MdiTextEditor *currentDoc = qobject_cast<MdiTextEditor*>(win->widget());
    if(currentDoc == NULL)
        return NULL;
    return currentDoc;
}

void Ide::on_actionStop_triggered()
{
    if(exec != NULL)
        exec->stopExec();
}

void Ide::on_actionForce_Stop_triggered()
{
    if(exec != NULL)
        exec->forceStop();
}

void Ide::on_actionRestore_triggered()
{
    //TODO Put "flow" in define so it will not be hardcoded
    QString path = whcFile.remove(whcFile.split("/").last()) + "flow";
    QFile flow(path);
    QTextStream readStream(&flow);
    QLinkedList<Exclusion> exclusionList;
    Exclusion excl;
    QString skip;

    flow.open(QIODevice::ReadOnly);

    while(!readStream.atEnd())
    {
        readStream>>excl.taskId;
        readStream>>skip;
        readStream>>excl.inFile;
        readStream>>skip;
        readStream>>excl.outFile;

        exclusionList.append(excl);
    }

    /**
     * Removing the last entry because it's empty (the wile loop executes
     * one more time after the last line, because there is still a \n left
     * before the end-of-file.
     */
    exclusionList.removeLast();

    flow.close();

    startProjectExec(QIODevice::Append, exclusionList);
}
