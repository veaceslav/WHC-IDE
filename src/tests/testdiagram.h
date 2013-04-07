/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
 * Copyright (C) 2013 by Andrei - Marius Dincu <andrei dot dincu at gmail dot com>
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

#ifndef TESTDIAGRAM_H
#define TESTDIAGRAM_H
#include <QObject>

class DiagramNode;

class TestDiagram : public QObject
{
    Q_OBJECT

private slots:
    void slotTestAddTask();
    void slotTestAddData();
    void slotTestDeleteItemById();
    void slotTestSearchById();
};

#endif // TESTDIAGRAM_H
