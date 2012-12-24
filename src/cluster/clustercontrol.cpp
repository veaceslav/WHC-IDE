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

#include "clustercontrol.h"
#include "ui_clustercontrol.h"


ClusterControl::ClusterControl(ClusterSettings clusterSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClusterControl)
{
    int ret;

    ui->setupUi(this);
    this->clusterSettings = clusterSettings;
    this->ui->qlabelInfo->setText("Connected to " + clusterSettings.username
                                  + "@" + clusterSettings.name);

    this->ui->qtextLogCluster->setText("Server IP : " + clusterSettings.serverIP + "\n"
                                       + "Server Dir : " + clusterSettings.serverDir + "\n\n"
                                       + "Sentinel IP : " + clusterSettings.sentinelIP + "\n"
                                       + "Sentinel Dir : " + clusterSettings.sentinelDir + "\n");

    ///////////////////////////////////////////////
    //
    //  TODO change code
    //
    ///////////////////////////////////////////////

    this->ui->qtextLogCluster->append("\nTest SSH Server : \nssh "
                                      + clusterSettings.username + "@" +clusterSettings.serverIP);
    ret = system(("ssh " + clusterSettings.username + "@" +clusterSettings.serverIP + " uname").toLatin1());
    if( ret == 0 )
        this->ui->qtextLogCluster->append("OK");
    else
        this->ui->qtextLogCluster->append("ERR connecting");

    this->ui->qtextLogCluster->append("\nTest SSH Sentinel : \nssh "
                                      + clusterSettings.username + "@" +clusterSettings.sentinelIP);
    ret = system(("ssh " + clusterSettings.username + "@" +clusterSettings.sentinelIP + " uname").toLatin1());
    if( ret == 0 )
        this->ui->qtextLogCluster->append("OK");
    else
        this->ui->qtextLogCluster->append("ERR connecting");
 }

ClusterControl::~ClusterControl()
{
    delete ui;
}

void ClusterControl::on_qbtnCancel_clicked()
{
    this->close();
}

void ClusterControl::on_qbtnEdit_clicked()
{
    this->close();
}

void ClusterControl::on_qbtnDisconnect_clicked()
{
    this->close();
}

void ClusterControl::on_qSelectProjDir_clicked()
{
    QString stringDir = QFileDialog::getExistingDirectory(0, "Select Project Dir");
    if(stringDir.isNull())
        return;

    this->ui->qlineProjectDir->setText(stringDir);
}

void ClusterControl::on_qbtnRequest_clicked()
{

    int ret;
    QString projectName;

    if(this->ui->qlineProjectDir->text() == "")
        return;

    projectName = this->ui->qlineProjectDir->text().split('/').last();
    ///////////////////////////////////////////////
    //
    //  TODO change code
    //
    ///////////////////////////////////////////////

    switch(this->ui->qcomboActions->currentIndex())
    {
        ///////////////////////////////////////////////////////
        // request estimation
        case 0:
        {
        this->ui->qtextLogCluster->append("\nRequesting estimation : scp -r "+
                                          this->ui->qlineProjectDir->text() + " " +
                                          clusterSettings.username + "@" +clusterSettings.sentinelIP +
                                          ":" + clusterSettings.sentinelDir);
        ret = system(("scp -r "+ this->ui->qlineProjectDir->text() + " " +
                      clusterSettings.username + "@" +clusterSettings.sentinelIP + ":" +
                      clusterSettings.sentinelDir + projectName + ".tmp").toLatin1());
        if( ret == 0 )
            this->ui->qtextLogCluster->append("OK");
        else
            this->ui->qtextLogCluster->append("ERR connecting");

        this->ui->qtextLogCluster->append("\nFinalize request : ssh -r "+
                                          clusterSettings.username + "@" +clusterSettings.sentinelIP +
                                          " mv " + clusterSettings.sentinelDir + projectName + ".tmp "
                                          + clusterSettings.sentinelDir + projectName);
        ret = system((" ssh "+ clusterSettings.username + "@" +clusterSettings.sentinelIP +
                      " mv " + clusterSettings.sentinelDir + projectName + ".tmp "
                      + clusterSettings.sentinelDir + projectName).toLatin1());
        }
        break;

        ///////////////////////////////////////////////////////
        // request processing
        case 1:
        {
        this->ui->qtextLogCluster->append("\nRequesting processing: scp -r "+
                                          this->ui->qlineProjectDir->text() + " " +
                                          clusterSettings.username + "@" +clusterSettings.serverIP +
                                          ":" + clusterSettings.serverDir);
        ret = system(("scp -r "+ this->ui->qlineProjectDir->text() + " " +
                      clusterSettings.username + "@" +clusterSettings.serverIP + ":" +
                      clusterSettings.serverDir + projectName + ".tmp").toLatin1());
        if( ret == 0 )
            this->ui->qtextLogCluster->append("OK");
        else
            this->ui->qtextLogCluster->append("ERR connecting");

        this->ui->qtextLogCluster->append("\nFinalize request : ssh "+
                                          clusterSettings.username + "@" +clusterSettings.serverIP +
                                          " mv " + clusterSettings.serverDir + projectName + ".tmp "
                                          + clusterSettings.sentinelDir + projectName);
        ret = system((" ssh "+ clusterSettings.username + "@" +clusterSettings.serverIP +
                      " mv " + clusterSettings.serverDir + projectName + ".tmp "
                      + clusterSettings.serverDir + projectName).toLatin1());
        }
        break;

        ///////////////////////////////////////////////////////
        // retrieve estimation
        case 2:
        {
        this->ui->qtextLogCluster->append("\nRequesting result estimation: scp "+
                                          clusterSettings.username + "@" +clusterSettings.sentinelIP +
                                          ":" + clusterSettings.sentinelDir + projectName + ".est " + this->ui->qlineProjectDir->text());
        ret = system(("scp "+clusterSettings.username + "@" +clusterSettings.sentinelIP +
                      ":" + clusterSettings.sentinelDir + projectName + ".est " + this->ui->qlineProjectDir->text() ).toLatin1());
        if( ret == 0 )
            this->ui->qtextLogCluster->append("OK");
        else
            this->ui->qtextLogCluster->append("ERR connecting");
        }
        break;

        ///////////////////////////////////////////////////////
        // retrieve processing
        case 3:
        {
        this->ui->qtextLogCluster->append("\nRequesting result processing: scp -r"+
                                          clusterSettings.username + "@" +clusterSettings.serverIP +
                                          ":" + clusterSettings.serverDir + projectName + ".done " + this->ui->qlineProjectDir->text());
        ret = system(("scp -r "+
                      clusterSettings.username + "@" +clusterSettings.serverIP +
                      ":" + clusterSettings.serverDir + projectName + ".done " + this->ui->qlineProjectDir->text()).toLatin1());
        if( ret == 0 )
            this->ui->qtextLogCluster->append("OK");
        else
            this->ui->qtextLogCluster->append("ERR connecting");
        }
        break;
    }
}
