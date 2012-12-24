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

#include "ui_addcluster.h"
#include "addcluster.h"
#include<QDebug>

AddCluster::AddCluster(ClusterSettings* clusterSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCluster)
{
    ui->setupUi(this);
    this->clusterSettings = clusterSettings;
    if(clusterSettings == NULL)
        return;

    // set gui fields
    ui->qlineName->setText( clusterSettings->name );
    ui->qlineServerIP->setText( clusterSettings->serverIP );
    ui->qlineSentinelIP->setText( clusterSettings->sentinelIP );
    ui->qlineServerDir->setText( clusterSettings->serverDir );
    ui->qlineSentinelDir->setText( clusterSettings->sentinelDir );
    ui->qlineUsername->setText( clusterSettings->username );
    ui->qlinePassword->setText( clusterSettings->password );
}

AddCluster::~AddCluster()
{
    delete ui;
    delete this->clusterSettings;
}

void AddCluster::on_AddCluster_accepted()
{
    clusterSettings->name = ui->qlineName->text();
    clusterSettings->serverIP = ui->qlineServerIP->text();
    clusterSettings->sentinelIP = ui->qlineSentinelIP->text();
    clusterSettings->serverDir = ui->qlineServerDir->text();
    clusterSettings->sentinelDir = ui->qlineSentinelDir->text();
    clusterSettings->username = ui->qlineUsername->text();
    clusterSettings->password = ui->qlinePassword->text();
}
