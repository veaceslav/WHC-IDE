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

#ifndef COmmanDLINE_H
#define COmmanDLINE_H

#include <QWidget>
#include "test/testCommandLine.h"

class QPlainTextEdit;
class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
class Ide;

class CommandLine : public QWidget
{
    Q_OBJECT
public:
    explicit CommandLine(Ide *parent = 0);
    
    void addLine(QString line, QColor txtColor);

    void addDebugLine(QString line);

    void showM();

    void clearAll();

    bool isVisible();

    //QPlainTextEdit* getWindow() {return window;}

private slots:
    void activateDebug();

private:

    Ide* parentIde;
    QPlainTextEdit* window;
    QToolButton* closeButton;
    QToolButton* debugButton;

    QHBoxLayout* lay;
    QVBoxLayout* vlay;
    bool visible;
    bool debugMode;

    friend class TestCommandLine;
    
};

#endif // COmmanDLINE_H
