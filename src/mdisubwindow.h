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

#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QMdiSubWindow>
#include <QFileInfo>
#include "mditexteditor.h"
#include "highlighter.h"

class MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit MdiSubWindow(const QString &fileName,
                          QWidget *parent = 0,
                          Qt::WindowFlags flags = 0);
    ~MdiSubWindow();
    bool loadFile(const QString &fileName);
    bool save();
    QString userFriendlyCurrentFile();
    QString currentFile();
    bool saveDocument();
    inline bool isModified() const {return modified;}
    inline void setUnmodified(){getEditor()->document()->setModified(false);}
    /**
     * \brief returns null if the file type is not known, for now detects the file type
     * only by extension
     */
    QString getFileType(QString filename);
    inline QString getFilePath() const {return fileName;}
    inline MdiTextEditor* getEditor() const {return textEditor;}
    virtual void closeEvent(QCloseEvent *closeEvent);
    QWidget *p;
signals:

public slots:
    void on_document_modify(bool modified);

private slots:
    void documentWasModified();

protected:
//    virtual void closeEvent(QCloseEvent *);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    QString fileName;
    MdiTextEditor *textEditor;
    bool modified;
};

#endif // MDISUBWINDOW_H
