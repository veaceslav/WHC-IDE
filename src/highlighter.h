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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDomDocument>
#include <QDomNode>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>
#include <QDomDocument>
#include <QMap>
#include <QDir>
#include <QStringList>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QCoreApplication>
#include <QXmlStreamWriter>


#include "mditexteditor.h"
#include "mdisubwindow.h"


#define HIGHLIGHTER_LANG_FOLDER "Global_settings/lang/"
#define HIGHLIGHTER_FORMAT_FOLDER "Global_settings/lang/format_scheme/"

class SyntaxHighlighter;
class MdiSubWindow;
class Ide;
typedef QPair<QString, QVector<QRegExp> > Type;
typedef QPair<QString, QTextCharFormat> Style;

/**
  * \class highlighter Is the main instance for a language's highight.
  * In the constructor are loaded the data about a certain language,
  * nad then with the highlight() method are applied on a certain subwindow.
  */
class Highlighter : public QObject{
    Q_OBJECT
public:
    Highlighter(QString filetype, EditorSettingsVar *es);
    ~Highlighter();
    /**
      * \Return Returns an attribute for a MditextEditor with the settings
      * from current highlight instance
      */
    SyntaxHighlighter *highlight(MdiSubWindow *child);

    /**
      * \return Returns the groups of keywords
      */
    inline QVector<Type*> getTypes() const {return types;}

    /**
      * \brief Return the current styles for highlights
      */
    inline QVector<Style*> getStyles() const {return styles;}

    /**
      * \brief Sets the name of the language
      */
    inline void setName(QString name){this->name = name;}

    /**
      * \return Returns the name of the language
      */
    inline QString getName() const {return name;}

    /**
      * \return Returns the keyowrd for a specific goup
      * \param key Is the name of the group
      */
    QVector<QRegExp> getType(QString &key);

    /**
      * \return Returns the style for a specific group
      * \param key Is the name of the group
      */
    QTextCharFormat getStyle(QString &key);

    /**
      * \brief Set style for a specific group to cf
      * \param key the group
      * \param cf the style
      */
    void setStyle(QString &key, QTextCharFormat &cf);

    /**
      * \brief Read the licence for the highlight xml support files
      */
    QString getLicence() const;

    SyntaxHighlighter *sh;

private:

    /**
      * \brief Reads syntax highlighting rules for a specific language
      */
    void readLanguages(const QDomDocument &doc);

    /**
      * \brief Reads details about the font used on highlight
      */
    void readFormat(const QDomDocument &doc);

    /**
      * \param types The groups of keywords
      */
    QVector<Type*> types;
    /**
      * \param styles The styles for each group
      */
    QVector<Style*> styles;
    /**
      * \param name The name of the language
      */
    QString name;

    EditorSettingsVar *es;
};

#endif // SETTINGS_H
