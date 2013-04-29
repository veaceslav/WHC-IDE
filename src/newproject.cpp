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

#include "newproject.h"


NewProject::NewProject(Ide *parent)
    :QDialog(parent)

{
    ui.setupUi(this);
    this->parent=parent;
    ui.projectLocation->setText(QDir::homePath());
    connect(ui.setprojectLocation, SIGNAL(clicked()),
            this, SLOT(slotSelectLocation()));
    connect(ui.templateProj, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotUpdateImage()));

    connect(ui.projectFinish, SIGNAL(clicked()),
            this, SLOT(slotCreateProject()));
}

void NewProject::slotUpdateImage()
{
    ui.templateImage->setCurrentIndex(ui.templateProj->currentIndex());
}

void NewProject::slotSelectLocation()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                              QDir::homePath(),
                                              QFileDialog::ShowDirsOnly |
                                              QFileDialog::DontResolveSymlinks);

    ui.projectLocation->setText(dir);

}

void NewProject::slotCreateProject()
{

    QString path  = ui.projectLocation->text();
    QString projName = ui.projectName->text();
    if(path.isEmpty())
    {
        QMessageBox msg;
        msg.setText("No project location specified.");
        msg.setInformativeText("Please select project location.");
        msg.exec();
        return;
    }
    if(projName.isEmpty())
    {
        QMessageBox msg;
        msg.setText("Project Name is Empty.");
        msg.setInformativeText("Please choose a name for your project");
        msg.exec();
        return;
    }
    QDir dir;
    /** Building file path: dir + "/" + filename **/
    path.append("/");
    path.append(projName);
    /** project Directory **/
    dir.mkdir(path);
    /** source and data subDirs **/
    dir.mkdir(path + "/src");
    dir.mkdir(path + "/data");

    /** Project .whc file **/
    path.append("/");
    path.append(projName);
    path.append(".whc");
    QFile file(path);

    file.open(QFile::WriteOnly);

    /** Creating xml Object **/
    QDomElement  projN;
    QDomDocument xmlDoc("WHC");

    projN = xmlDoc.createElement("project");
    xmlDoc.appendChild(projN);
    projN.setAttribute(QString("name"),projName);

    QDomElement task = xmlDoc.createElement("tasks");
    QDomElement files = xmlDoc.createElement("data");
    QDomElement diag  = xmlDoc.createElement("diagram");

    projN.appendChild(task);
    projN.appendChild(files);
    projN.appendChild(diag);


    /** Write data to file **/
    QByteArray data(xmlDoc.toByteArray());
    QDataStream stream( &file );
    stream.writeRawData(data.data(), data.size());
    file.close();

    parent->startNewProject(path);
    this->close();
}
NewProject::~NewProject()
{

}
