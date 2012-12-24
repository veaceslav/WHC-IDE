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

#ifndef CLUSTERSETTINGS_H
#define CLUSTERSETTINGS_H

#include <QString>
#include <QDomDocument>
#include <QDomNode>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>
#include <QDomDocument>
#include <QMap>
#include <QStringList>
#include <QColor>
#include <QDebug>

class ClusterSettings
{
public:
    QString name;
    QString serverIP;
    QString sentinelIP;
    QString serverDir;
    QString sentinelDir;
    QString username;
    QString password;

    // default constructor
    ClusterSettings();

    };

#endif // CLUSTERSETTINGS_H
