/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2013 by Andrei Preda <andreipreda07 at gmail dot com>
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

#include "projManagement/overwritefile.h"
#include "staticmethods.h"

OverwriteFile::OverwriteFile(QDialog *parent, QString fileName)
    :QDialog(parent)
{
    this->resize(390, 148);
    this->setWindowTitle("Add New File");

    overrideFileChoice = new QDialogButtonBox(this);
    overrideFileChoice->setGeometry(QRect(40, 110, 341, 32));
    overrideFileChoice->setOrientation(Qt::Horizontal);
    overrideFileChoice->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    warnLabel = new QLabel(this);
    warnLabel->setGeometry(QRect(10, 20, 300, 42));
    warnLabel->setText("File \"" + fileName + "\" already exists.\n" +
                       "Proceeding will overwrite the old file!");

    QObject::connect(overrideFileChoice, SIGNAL(accepted()),
                     this, SLOT(slotCanOverwrite()));
    QObject::connect(overrideFileChoice, SIGNAL(rejected()),
                     this, SLOT(slotCannotOverwrite()));
}

OverwriteFile::~OverwriteFile()
{
    StaticMethods::destroyObj(&overrideFileChoice);
    StaticMethods::destroyObj(&warnLabel);
}

void OverwriteFile::slotCannotOverwrite()
{
    emit signalOvewriteRejected();
    this->close();
}

void OverwriteFile::slotCanOverwrite()
{
    emit signalOvewriteAccepted();
    this->close();
}

