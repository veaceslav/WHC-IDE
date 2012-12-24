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

#ifndef ADDFILES_H
#define ADDFILES_H
#include "ide.h"
#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDomDocument>

#ifdef _WIN32
#define sep "\\"
#elif __linux__
#define sep "/"
#endif


class Ide;
class ProjectTreeItem;

class AddFiles : public QDialog
{
  Q_OBJECT

public:
    AddFiles(QDomDocument* proj, Ide* parent, QModelIndex selected);
    ~AddFiles();

private slots:
    /**
     * @brief slotGetFiles - open a dialog
     *                      to select files to add
     */
    void slotGetFiles();
    /**
     * @brief slotAddFiles -add selected fiels to .whc
     */
    void slotAddFiles();

private:

    /**
     * @brief copyFiles - add selected files to project directory
     */
    void copyFiles();

    QList<QString> files;
    QDomNodeList lst;
    QDomDocument* projectXml;
    ProjectTreeItem* tasksItem;
    Ide* parent;
    /**
     * Gui elements
     */
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QComboBox *comboBox;
    QPushButton *pushButton;
};
#endif // ADDFILES_H
