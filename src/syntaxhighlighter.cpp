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

#include "syntaxhighlighter.h"


SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent,
                                     const  QVector<QPair<QVector<Type*> , QVector<Style*> >* > &rules,
                                     EditorSettingsVar *es)
    : QSyntaxHighlighter(parent)
{
    this->types = rules[0]->first;
    this->styles = rules[0]->second;
    this->es = es;
    endSpace = new QRegExp("\\s+$");
    endSpaceFormat.setBackground(QBrush(QColor("#FFCC66")));
}

SyntaxHighlighter::~SyntaxHighlighter()
{
    Ide::destroyObj(&endSpace);
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    int index;
    if(es->endLineSpace){
        index = endSpace->indexIn(text);
        if(index != -1){
            setFormat(index, endSpace->matchedLength(), endSpaceFormat);
        }
    }

    for(int i = 0; i < types.size(); i++){
        for(int j = 0; j < types[i]->second.size(); j++){
            if(previousBlockState() != 1 ||
                    types[i]->first.startsWith("end")
                    || currentBlockState() == 2){
                QRegExp expression = (types[i]->second)[j];
                index = expression.indexIn(text);

                if(types[i]->first.startsWith("end") && index > -1)
                    setCurrentBlockState(2);

                if(types[i]->first.startsWith("start") && index > -1){
                    setCurrentBlockState(1);
                    format = styles[i]->second;
                }
                while(index >= 0){
                    int length = expression.matchedLength();
                    setFormat(index, length, styles[i]->second);
                    index = expression.indexIn(text, index+length);
                }
            }
            else{
                setFormat(0, text.size(), format);
                setCurrentBlockState(1);
            }
        }
    }

}

void SyntaxHighlighter::setStyle(QString &key, QTextCharFormat &cf)
{
    for(int i = 0; i < styles.size(); i++){
        if(styles[i]->first == key){
            styles[i]->second.setForeground(cf.foreground());
            this->rehighlight();
            return;
        }
    }
}
