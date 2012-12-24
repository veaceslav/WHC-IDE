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

#ifndef ADDCLUSTER_H
#define ADDCLUSTER_H

#include <QDialog>
#include "clusterconnect.h"
#include "settings/clustersettings.h"

namespace Ui {
    class AddCluster;
}

class AddCluster : public QDialog
{
    Q_OBJECT

public:
    ClusterSettings* clusterSettings;
    explicit AddCluster(ClusterSettings* clusterSettings, QWidget *parent = 0);
    ~AddCluster();

private slots:

    void on_AddCluster_accepted();

private:
    Ui::AddCluster *ui;
};

#endif // ADDCLUSTER_H
