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

#include "clusterconnect.h"
#include "ui_clusterconnect.h"

ClusterConnect::ClusterConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClusterConnect)
{
    ui->setupUi(this);

}

ClusterConnect::~ClusterConnect()
{
    delete ui;
}

void ClusterConnect::on_qbtnCancel_clicked()
{
    this->close();
}

void ClusterConnect::on_qbtnEdit_clicked()
{
    //AddCluster addCluster;
    //addCluster.exec();
}

void ClusterConnect::on_qbtnAdd_clicked()
{
    ClusterSettings* newCluster = new ClusterSettings;
    AddCluster* addCluster = new AddCluster(newCluster);
    if(addCluster->exec() == true){
        this->listClusterSettings.push_back(*newCluster);
        this->ui->qlistClusters->addItem(newCluster->name);
    }
    delete newCluster;
}

void ClusterConnect::on_qbtnConnect_clicked()
{
    // lookup specific cluster configuration and apply it for connecting
    for(int i=0; i<listClusterSettings.size(); i++)
        if(ui->qlistClusters->currentText() == listClusterSettings[i].name)
        {
            this->close();
            ClusterControl* clusterControl = new ClusterControl(listClusterSettings[i]);
            clusterControl->exec();
            break;
        }
}

void ClusterConnect::on_qlistClusters_currentIndexChanged(int index)
{
    if(index == -1){
        ui->qbtnEdit->setEnabled(false);
        ui->qbtnConnect->setEnabled(false);
    }
    else
    {
        ui->qbtnEdit->setEnabled(true);
        ui->qbtnConnect->setEnabled(true);
    }
}
