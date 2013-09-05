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

#ifndef MDICHILD_H
#define MDICHILD_H

#include <QTextEdit>
#include <QCloseEvent>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QRect>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QCompleter>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QStringListModel>
#include <QDebug>

#include "ide.h"

/**
  Line number as in doc.trolltech.com
  http://doc.trolltech.com/4.6/widgets-codeeditor.html
*/
class Ide;

class MdiTextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    MdiTextEditor(const QString &fileName, QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    void replaceSpacesLineEnd();
    void goTo(int to);

protected:
    void resizeEvent(QResizeEvent *event);
    void focusInEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void bracketMatch();
    void slotIndent();
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect &, int);
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;
    QAbstractItemModel *modelFromFile();
    QWidget *lineNumberArea;
    QCompleter *c;
    Ide *ide;
    /**
     * @brief oldSize - size of the text before modification
     */
    int oldSize;
    QStringListModel *completionModel;
    QStringList words;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(MdiTextEditor *editor) : QWidget(editor)
    {
        codeEditor = editor;
    }

    QSize sizeHint() const
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    MdiTextEditor *codeEditor;
};

#endif
