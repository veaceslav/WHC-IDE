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

#ifndef DATAGROUP_H
#define DATAGROUP_H
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "ide.h"


class Ide;
class DataGroup : public QDialog
{
    Q_OBJECT

public:
    DataGroup(QDomDocument* proj,Ide* parent);
    ~DataGroup();


private slots:

    void slotDataGroup();

private:
    QDomDocument* projectXml;
    Ide* parent;

    /**
     *  Gui elements
     */
    QDialogButtonBox *buttonBox;
    QLabel *taskLabel;
    QLineEdit *taskName;
    QVBoxLayout verticalLayout;
    QGridLayout gridLayout;
    QHBoxLayout orizontalLayout;

};

#endif //DATAGROUP_H
