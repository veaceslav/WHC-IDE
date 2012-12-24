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

#ifndef CLUSTERCONNECT_H
#define CLUSTERCONNECT_H

#include <QDialog>
#include "addcluster.h"
#include "clustercontrol.h"
#include "settings/clustersettings.h"

namespace Ui {
    class ClusterConnect;
}

class ClusterConnect : public QDialog
{
    Q_OBJECT

public:
    QList<ClusterSettings> listClusterSettings;
    explicit ClusterConnect(QWidget *parent = 0);
    ~ClusterConnect();

private slots:
    void on_qbtnCancel_clicked();
    void on_qbtnEdit_clicked();
    void on_qbtnAdd_clicked();
    void on_qbtnConnect_clicked();

    void on_qlistClusters_currentIndexChanged(int index);

private:
    Ui::ClusterConnect *ui;
};

#endif // CLUSTERCONNECT_H
