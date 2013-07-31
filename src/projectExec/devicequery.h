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

#ifndef DEVICEQUERY_H
#define DEVICEQUERY_H

#include <QWidget>
#include <QList>
#include <QListWidgetItem>

#define MAX_PLAT 10
#define MAX_DEV  40
#define MAX_STR  100

class Ide;
class QListWidget;
class QToolButton;
class QLabel;

class DeviceQuery : public QWidget
{
    Q_OBJECT
public:
    DeviceQuery(Ide* parent);

    /**
     * @brief showDev - show device list
     */
    void showDev();

    QVector<int> getSelection();

    /**
     * @brief devicesCount - gets the number of devices
     * @return        - returns the number of devices
     */
    int devicesCount();

    /**
     * @brief getName - get the name of the requested device
     * @param devId   - the id of the device from the widget
     * @return        - returns the name of the id as it appears in the devList
     */
    QString getName(int devId);

public slots:
    /**
     * @brief hideDev - hide device list when clicking close button
     */
    void hideDev();
    /**
     * @brief getDeviceInfo -run OpenCl code to list available platforms and
     *                       devices
     */
    void getDeviceInfo();
private:

    Ide *parentIde;
    QListWidget *devList;
    QToolButton *closeButton;
    QToolButton *refresh;
    QLabel *title;

    QList<QListWidgetItem*> items;
};

#endif // DEVICEQUERY_H
