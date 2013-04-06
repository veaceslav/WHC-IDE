/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
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

#ifndef TESTDIAGRAMSCENE_H
#define TESTDIAGRAMSCENE_H

#include <QObject>


class TestDiagramScene : public QObject
{
    Q_OBJECT
public:
    explicit TestDiagramScene(QObject *parent = 0);
    
private slots:
    void slotTestRenameItems();
    void slotTestGetItemById();
    void slotTestDeleteItemsById();
    void slotTestRemoveFromList();
    void slotTestLoadDiagram();
    
};

#endif // TESTDIAGRAMSCENE_H
