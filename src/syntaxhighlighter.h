/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
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

#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H
#include "highlighter.h"
#include <QSyntaxHighlighter>
#include <QFile>
class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent,
                      const  QVector<QPair<QVector<Type*>, QVector<Style*> >* > &rules,
                      EditorSettingsVar *es);
    ~SyntaxHighlighter();
	void setStyle(QString &key, QTextCharFormat &form);
    QVector<Type*> types;
    QVector<Style*> styles;
    QTextCharFormat format;

protected:
    void highlightBlock(const QString &cf);
private:
    QRegExp* endSpace;
    QTextCharFormat endSpaceFormat;
    EditorSettingsVar *es;
};


#endif // HIGHLIGHT_H
