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

#ifndef ADDDATA_H
#define ADDDATA_H
#include "ide.h"
#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDomDocument>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#ifdef _WIN32
#define sep "\\"
#elif __linux__
#define sep "/"
#endif


class Ide;
class ProjectTreeItem;

class AddData : public QDialog
{
  Q_OBJECT

public:
    AddData(QDomDocument* proj, Ide* parent, QModelIndex selected);
    ~AddData();

private slots:
    /**
     * @brief slotGetFiles - open a dialog
     *                      to select files to add
     */
    void slotGetFiles();
    /**
     * @brief slotAddData -add selected fiels to .whc
     */
    void slotAddData();

private:

    /**
     * @brief copyFiles - add selected files to project directory
     */
    void copyFiles();

    QList<QString> files;
    QDomNodeList lst;
    QDomDocument* projectXml;
    Ide* parent;
    ProjectTreeItem* dataItem;
    /**
     * Gui elements
     */
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QVBoxLayout verticalLayout;
    QGridLayout gridLayout;
    QHBoxLayout orizontalLayout;
};
#endif // ADDDATA_H
