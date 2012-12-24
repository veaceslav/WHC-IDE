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

#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDomText>

#include "ide.h"

class Ide;

class NewProject : public QDialog
{
    Q_OBJECT

public:

    NewProject(Ide *parent);
    ~NewProject();

    /**
     * GUI elements
     */
    Ide *parent;
    QWidget *layoutWidget;
    QHBoxLayout *buttonLayout;
    QPushButton *cancelButton;
    QPushButton *finishButton;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *projNameLabel;
    QLineEdit *projectName;
    QFrame *line;
    QHBoxLayout *horizontalLayout_5;
    QLabel *locationLabel;
    QLineEdit *projectLocation;
    QPushButton *setprojectLocation;
    QTextEdit *templateInfo;
    QLabel *label_4;
    QStackedWidget *templateImage;
    QWidget *page;
    QLabel *label_6;
    QWidget *page_3;
    QLabel *label_8;
    QWidget *page_4;
    QLabel *label_9;
    QWidget *page_5;
    QLabel *label_3;
    QComboBox *templateProj;
    QFrame *line_2;

private Q_SLOTS:

    /**
     * Link switching image from the left side
     * with options from QComboBox
     */
    void slotUpdateImage();

    /**
     * Change text from QTextEdit according with
     * option selected in QComboBox
     */
    void slotUpdateTemplateInfo();

    /**
     * Open a folder select window
     */
    void slotSelectLocation();

    /**
     * Create a new .whc file
     * on specified location
     */
    void slotCreateProject();
};
#endif // NEWPROJECT_H
