/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Cosmin Mihai <yonutix.mc@gmail.com>
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

#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H
#include <QPlainTextEdit>
#include <QColorDialog>
#include "ide.h"
#include "generalsettings.h"
#include "mdisubwindow.h"
#include "syntaxhighlighter.h"

class QObject;
class Ide;

class EditorSettings : QObject
{
    Q_OBJECT
public:
    EditorSettings(Ide *parent, Ui::settingsDialog *win);
    ~EditorSettings();

    /**
     * @return Returns the current class for highlight
     */
    inline Highlighter *getCurrentHighlighter() const
            {return parent->langs[win->comboBox->currentText()];}

    /**
     * @brief Actions that happends on <<apply>> button pressed
     */
    void applyButtonPressed();

private slots:
    /**
     * @brief on Replace tabs button checkbox checked or unchecked
     */
    void checkButtonChanged(int state);

    /**
     * @brief Reset editor settings to default
     */
    void resetSettings();


    /**
     * @brief On Language keywords group selection changed
     */
    void listItemChanged(QListWidgetItem *item);

    /**
     * @brief Change color on request
     */
    void onChooseCOlorClicked();

    /**
     * @brief On Highlight line ending state changed
     */
    void endLineHighlightChange(int state);

private:
    /**
     * @param parent Main class of the IDE
     */
    Ide* parent;

    /**
     * @param win Pointer to program form
     */
    Ui::settingsDialog *win;
    QWidget *colors;
    QWidget *fonts;
    QHash<QString, QString> langs;

    /**
     * @brief Apply color selected
     */
    void setColorsTab();

    /**
     * @brief Fill tab width form with the current tab width
     */
    void setFontTab();

    /**
     * @brief Load settings from global settings
     */
    void loadSettings();

    /**
     * @brief Save settings to global variables
     */
    void saveSettings();

    /**
     * @brief Apply settings to editor
     */
    void applySettings();

    /**
     * @brief check if the settings are valid
     */
    bool checkSettings();

    /**
     * @brief Save only the highlight configuration
     */
    void saveHighlightSettings();

    /**
     * @brief Load the highlight configuration
     */
    void loadHighlight(QString &lang);

    /**
     * @brief Load the groups of keywords that have to be
     * highlighted
     */
    void loadKeyWords(QVector<QRegExp> &w);


    /**
     * @brief Sets the tab size to tabsize
     */
    inline void setTabSize(int tabSize)
            {parent->editorSettings->tabSize = tabSize;}

    /**
     * @brief Sets the font size to fontSize
     */
    inline void setFontSize(int fontSize)
            {parent->editorSettings->fontSize = fontSize;}

    /**
     * @brief Sets the FontFamily to fontFamily
     */
    inline void setfontFamily(QString &fontFamily)
            {parent->editorSettings->fontFamily = fontFamily;}

    /**
     * @brief Sets <<Tab to spaces>> stat to tabToSpaces state
     */
    inline void setTabToSpaces(bool tabToSpaces)
            {parent->editorSettings->tabToSpaces = tabToSpaces;}

    /**
     * @return Return the tab size from the form
     */
    inline int getTabSize() const
            {return parent->editorSettings->tabSize;}

    /**
     * @return Returns the font family specified in the form
     */
    inline QString getFontFamily() const
            {return parent->editorSettings->fontFamily;}

    /**
     * @return Returns the font size specified in the editor settings form
     */
    inline int getFontSize() const
            {return parent->editorSettings->fontSize;}

    /**
     * @return Return the state of <<tab to spaces>>
     */
    inline bool getTabToSpaces() const
            {return parent->editorSettings->tabToSpaces;}

};

#endif // EDITORSETTINGS_H
