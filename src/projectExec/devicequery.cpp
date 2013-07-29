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

#include "devicequery.h"
#include "cl.h"
#include "ide.h"
#include <QDebug>
#include <QListWidget>
#include <QToolButton>
#include <QLabel>

DeviceQuery::DeviceQuery(Ide *parent)
    :QWidget(parent), parentIde(parent)
{
    this->setMaximumHeight(250);

    devList = new QListWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(this);

    QHBoxLayout *hb = new QHBoxLayout();

    lay->addLayout(hb);

    title = new QLabel("Press refresh");

    QIcon ref(":/images/Refresh48x48.gif");
    QIcon closeIcon(":/images/close2.png");
    refresh = new QToolButton(this);
    refresh->setIcon(ref);
    connect(refresh, SIGNAL(clicked()), this, SLOT(getDeviceInfo()));

    closeButton = new QToolButton(this);
    closeButton->setIcon(closeIcon);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hideDev()));

    hb->addWidget(title);
    hb->addWidget(refresh);
    hb->addWidget(closeButton);

    lay->addWidget(devList);
    this->setLayout(lay);

    showDev();


}

void DeviceQuery::getDeviceInfo()
{

    devList->clear();

    cl_platform_id platform[MAX_PLAT];
    cl_device_id devices[MAX_DEV];

    cl_uint totalDevices = 0;
    cl_uint platformDevices = 0;

    cl_uint num_platforms = 0;
    char tempString[MAX_STR];

    clGetPlatformIDs(MAX_PLAT, platform, &num_platforms);

    if(num_platforms == 0)
    {
        title->setText("No platforms found!");
        return;
    }

    for(unsigned int i = 0; i < num_platforms; i++)
    {
        QString code = "[";
        clGetPlatformInfo(platform[i], CL_PLATFORM_NAME,
                          sizeof(char) * MAX_STR, tempString, NULL);

        code+= QString(tempString).split(" ").first() + "]";

        clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_ALL, MAX_DEV, devices,
                       &platformDevices);

        totalDevices += platformDevices;

        for(unsigned int j = 0; j < platformDevices; j++)
        {
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME,
                            sizeof(char) * MAX_STR, tempString, NULL);

            QListWidgetItem *item1 = new QListWidgetItem(code
                                                         + QString(tempString));
            item1->setFlags(item1->flags() | Qt::ItemIsUserCheckable);
            item1->setCheckState(Qt::Checked);
            devList->insertItem(0, item1);
        }
    }
    title->setText(QString("Found " + QString::number(totalDevices)
                           + " device(s)"));
}

QVector<int> DeviceQuery::getSelection()
{
    QVector<int> selection;

    if(devList->count() == 0)
        getDeviceInfo();

    for(int i = 0; i < devList->count(); i++)
    {
        QListWidgetItem *item = devList->item(i);
        if(item->checkState() == Qt::Checked)
            selection.push_back(i);
    }
    return selection;
}

int DeviceQuery::devices()
{
    getDeviceInfo();
    return devList->count();
}

QString DeviceQuery::getName(int devId)
{
    QListWidgetItem *dev = devList->item(devId);
    if(!dev)
    {
        getDeviceInfo();
        dev = devList->item(devId);
    }
    if(dev)
        return dev->text();
    return QString();
}

void DeviceQuery::showDev()
{
    title->show();
    closeButton->show();
    refresh->show();
    devList->show();
    parentIde->addDeviceList(this);
}


void DeviceQuery::hideDev()
{
    parentIde->deleteDevWindow(this);
    title->hide();
    closeButton->hide();
    refresh->hide();
    devList->hide();
}
