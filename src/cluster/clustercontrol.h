/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C)
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

#ifndef CLUSTERCONTROL_H
#define CLUSTERCONTROL_H

#include <QDialog>
#include <QFileDialog>
#include "clusterconnect.h"
#include "settings/clustersettings.h"

namespace Ui {
    class ClusterControl;
}

class ClusterControl : public QDialog
{
    Q_OBJECT

public:
    ClusterSettings clusterSettings;
    explicit ClusterControl(ClusterSettings clusterSettings, QWidget *parent = 0);
    ~ClusterControl();

private slots:
    void on_qbtnCancel_clicked();
    void on_qbtnEdit_clicked();

    void on_qbtnDisconnect_clicked();

    void on_qSelectProjDir_clicked();

    void on_qbtnRequest_clicked();

private:
    Ui::ClusterControl *ui;
};

#endif // CLUSTERCONTROL_H
