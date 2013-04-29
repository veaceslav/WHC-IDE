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

#ifndef OVERWRITEFILE_H
#define OVERWRITEFILE_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>

class OverwriteFile : public QDialog
{
  Q_OBJECT

public:
    OverwriteFile(QDialog *parent, QString fileName);
    ~OverwriteFile();

signals:
    void signalOvewriteAccepted();
    void signalOvewriteRejected();

private slots:

    /**
     * @brief slotCanOverwrite -emits signalOvewriteAccepted
     */
    void slotCanOverwrite();
    /**
     * @brief slotCannotOverwrite -emits signalOvewriteRejected
     */
    void slotCannotOverwrite();
private:
    QDialogButtonBox *overrideFileChoice;
    QLabel *warnLabel;
};

#endif // OVERWRITEFILE_H
