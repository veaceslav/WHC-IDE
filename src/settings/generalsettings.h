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

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QString>
#include <QFile>
#include <QSettings>
#include "ide.h"
#include "ui_settingsDialog.h"
#include "projectsettings.h"
#include "editorSettings.h"

class Ide;
class EditorSettings;
class GeneralSettings : public QDialog
{
    Q_OBJECT

public:
    GeneralSettings(Ide *parent, ProjectSettings *settings);
    ~GeneralSettings();

private slots:

    /**
     * @brief slotUpdateWidget -update stackWidgets index when an element is
     *                         - clicked on QListWidget
     */
    void slotUpdateWidget();

    /** Button vsButton clicked **/
    void slotSetVSDir();

    /** Button sdkButton clicked **/
    void slotSetSDKDir();

    /** Button clButton clicked **/
    void slotSetClDir();

    /** Button libclButton clicked **/
    void slotSetLibClDir();

    /**
     * @brief slotSaveSettings - save all seettings into projectsettings class
     *                           after ok button was clicked
     *                           and call setSettingstoFile()
     */
    void slotSaveSettings();

private:
    /**
     * @brief setDir - open a Directory select dialog
     *                 and set path to given lineEdit
     * @param line   - lineEdit with directory path
     */
    void setDir(QLineEdit *line);
    /**
     * @brief loadSettings - if settings exists, load them from given
     *                       projectsettings object
     */
    void loadSettings();
    /**
     * @brief setSettingsToFile - store settings using QSettings
     */
    void setSettingsToFile();

    Ui::settingsDialog *ui;
    ProjectSettings *settings;
    EditorSettings *editor_settings;

};

#endif // GENERALSETTINGS_H
