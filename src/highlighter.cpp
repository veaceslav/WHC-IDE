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

#include "highlighter.h"
#include "staticmethods.h"
#include "syntaxhighlighter.h"

Highlighter::Highlighter(QString filetype, EditorSettingsVar *es)
{
    this->es = es;
    QDomDocument doc("HIGHLIGHT");
    sh = NULL;
    QFile file(QString(HIGHLIGHTER_LANG_FOLDER) + filetype + QString(".lang"));
    QFile file_f(QString(HIGHLIGHTER_FORMAT_FOLDER) + filetype +
                 QString(".lang"));
    if(!file.exists() || !file_f.exists())
    {
#ifndef DEBUG
        QMessageBox::warning(0, tr("Warning!"),
                             tr("Syntax highlighter definitions file does not exist.\n"
                                "Syntax highlighting will not be available."));
#endif
    }
    else if(!file.open(QFile::ReadOnly) || !file_f.open(QFile::ReadOnly))
    {
#ifndef DEBUG
        QMessageBox::warning(0, tr("Warning!"),
                             tr("Could not open syntax highlighter definitions.\n"
                                "Syntax highlighting will not be available."));
#endif
    }
    else
    {
        QString errorMsg;
        int errorLine, errorColumn;
        if(!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
        {
#ifndef DEBUG
            QMessageBox::warning(0, tr("Warning!"),
                                 tr("Error parsing the syntax highlighter definitions,\n"
                                    "%1 at line %2, column %3").arg(errorMsg)
                                        .arg(errorLine).arg(errorColumn));
#endif
        }
        else
        {
            readLanguages(doc);
        }

        if(!doc.setContent(&file_f, &errorMsg, &errorLine, &errorColumn))
        {
#ifndef DEBUG
            QMessageBox::warning(0, tr("Warning!"),
                                 tr("Error parsing the syntax highlighter definitions,\n"
                                    "%1 at line %2, column %3").arg(errorMsg)
                                        .arg(errorLine).arg(errorColumn));
#endif
        }
        else
        {
            readFormat(doc);
        }
    }
    file.close();
    file_f.close();
}

Highlighter::~Highlighter()
{
    StaticMethods::destroyObj(&sh);
    foreach(Type *t, types)
    {
        delete t;
    }
    foreach(Style *s, styles)
    {
        delete s;
    }
}

SyntaxHighlighter *Highlighter::highlight(MdiSubWindow *mdiSubWindow)
{
    QPair<QVector<Type*>, QVector<Style*> > content(types, styles);
    QVector<QPair<QVector<Type*>, QVector<Style*> >* > container;
    container.append(&content);
    QDir *dr = new QDir(".");
    Q_UNUSED(dr);
    sh = new SyntaxHighlighter(qobject_cast<MdiTextEditor*>
                               (mdiSubWindow->widget())->document(), container,
                               es);
    return sh;
}

void Highlighter::readFormat(const QDomDocument &doc)
{
    QDomElement root, pItemRoot;
    root = doc.documentElement();
    if(root.tagName() != "format")
    {
        QMessageBox::warning(0, tr("Warning!"),
                             tr("Error parsing the syntax highlighter definitions.\n"
                                "Syntax highlighting will not be available.\n"));
        return;
    }
    pItemRoot = root.firstChildElement().firstChildElement();

    while(!pItemRoot.isNull())
    {
        if(pItemRoot.tagName() == "color" &&
           pItemRoot.attribute("format") == "#")
        {
            QTextCharFormat text;
            text.setForeground(QBrush(QColor(pItemRoot.text())));
            styles.append(new QPair<QString,
                          QTextCharFormat>(pItemRoot.attribute("name"), text));
        }
        if(pItemRoot.tagName() == "bold")
        {
            styles[styles.size() - 1]->second.setFontWeight(QFont::Bold);
        }
        pItemRoot = pItemRoot.nextSiblingElement();
    }
}

void Highlighter::readLanguages(const QDomDocument &doc)
{
    QDomElement root, pItemRoot, pItem;
    // get root from file
    // check data consistency
    root = doc.documentElement();
    if(root.tagName() != "language")
    {
        QMessageBox::warning(0, tr("Warning!"),
                             tr("Error parsing the syntax highlighter definitions.\n"
                                "Syntax highlighting will not be available.\n"));
        return;
    }

    /** Starting parsing xml file **/
    pItemRoot = root.firstChildElement();

    while(!pItemRoot.isNull())
    {
        if(pItemRoot.nodeName() == "context")
        {
            QString type_id = pItemRoot.attribute(QString("id"));
            QVector<QRegExp> keywords;
            pItem = pItemRoot.firstChildElement();
            while(!pItem.isNull())
            {
                if(pItem.nodeName() == "keyword")
                {
                    QString s = pItem.text();
                    keywords.append(QRegExp(s, Qt::CaseSensitive));
                }
                pItem = pItem.nextSiblingElement();
            }
            types.append(new QPair<QString,
                         QVector<QRegExp> >(type_id, keywords));
        }
        /** Loop through elements **/
        pItemRoot = pItemRoot.nextSiblingElement();
    }
}

QVector<QRegExp> Highlighter::getType(QString &key)
{
    QVector<QRegExp> v;
    foreach(Type *t, types)
    {
        if(t->first == key)
            return t->second;
    }
    return v;
}

QTextCharFormat Highlighter::getStyle(QString &key)
{
    QTextCharFormat cf;
    if(sh == NULL)
    {
        foreach(Style *t, styles)
        {
            if(t->first == key)
                return t->second;
        }
    }
    else
    {
        foreach(Style *t, sh->styles)
        {
            if(t->first == key)
                return t->second;
        }
    }
    return cf;
}

void Highlighter::setStyle(QString &key, QTextCharFormat &cf)
{
    for(int i = 0; i < styles.size(); i++)
    {
        if(styles[i]->first == key)
        {
            styles[i]->second = cf;
            return;
        }
    }
}


QString Highlighter::getLicence() const
{
    QString licence;
    QFile f("Global_settings/langs_licence.txt");
    if(f.open(QIODevice::ReadOnly))
    {
        QTextStream in(&f);
        while(!in.atEnd())
            licence += in.readLine() + "\n";
        f.close();
    }
    return licence;
}
