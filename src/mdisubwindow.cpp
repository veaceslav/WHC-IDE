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

#include "mdisubwindow.h"

#include "ide.h"

MdiSubWindow::MdiSubWindow(const QString &fileName, QWidget *parent, Qt::WindowFlags flags) :
    QMdiSubWindow(parent, flags),fileName(fileName)
{
    this->p = parent;
    setAttribute(Qt::WA_DeleteOnClose);
    setCurrentFile(fileName);
    textEditor = new MdiTextEditor(fileName, this);
    setWidget(textEditor);
    modified = false;
    connect(getEditor()->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(on_document_modify(bool)));

    QString filetype = getFileType(fileName);
    if(filetype.compare("") != 0){
        Ide *p = reinterpret_cast<Ide*>(parent);
        if(!p->langs.contains(filetype)){
            p->langs.insert(filetype, new Highlighter(filetype, p->editorSettings));
           p->langs[filetype]->setName(filetype);
        }
        p->langs[filetype]->highlight(this);
    }
}

MdiSubWindow::~MdiSubWindow()
{
    delete textEditor;
}

QString MdiSubWindow::getFileType(QString filename)
{
    if(filename.endsWith(".cpp") || filename.endsWith(".hpp"))
        return QString("cpp");
    if(filename.endsWith(".c") || filename.endsWith(".h"))
        return QString("c");
    if(filename.endsWith(".cl"))
        return QString("opencl");
    if(filename.endsWith("Makefile"))
        return QString("makefile");
    return "";
}

void MdiSubWindow::on_document_modify(bool modified)
{
    if(modified){
        foreach(QTabBar * tab, this->mdiArea()->findChildren<QTabBar*>()){
            tab->setTabTextColor(tab->currentIndex(), Qt::red);
            //connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(on_tab_closed(int)));
            this->modified = true;
        }
    }
    else{
        this->modified = false;
        foreach(QTabBar * tab, this->mdiArea()->findChildren<QTabBar*>()){
            tab->setTabTextColor(tab->currentIndex(), Qt::black);
        }
    }
}
void MdiSubWindow::closeEvent(QCloseEvent *closeEvent)
{
    if(modified){
        QMessageBox::StandardButton answ = QMessageBox::question(this, tr("WHC"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(answ == QMessageBox::Save){
            saveDocument();
            closeEvent->accept();
        }
        if(answ == QMessageBox::Discard){
            closeEvent->accept();
        }
        if(answ == QMessageBox::Cancel){
            closeEvent->ignore();
        }
    }
    else{
        closeEvent->accept();
    }
}

void MdiSubWindow::documentWasModified()
{
    modified = true;
    setWindowModified(true);
}

bool MdiSubWindow::saveDocument()
{
    QFile f(getFilePath());
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&f);
    out << getEditor()->document()->toPlainText();
    f.close();
    return true;
}

void MdiSubWindow::setCurrentFile(const QString &fileName)
{
    this->fileName = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiSubWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString MdiSubWindow::userFriendlyCurrentFile()
{
    return strippedName(fileName);
}

QString MdiSubWindow::currentFile()
{
    return fileName;
}
