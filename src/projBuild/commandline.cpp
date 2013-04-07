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

#include "commandline.h"
#include <QPlainTextEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QDebug>
#include "../ide.h"

CommandLine::CommandLine(Ide *parent) :
    QWidget(0),parentIde(parent)
{

    debugMode = false;

    window = new QPlainTextEdit(this);
    window->setMaximumHeight(200);

    closeButton = new QToolButton();
    QIcon icon(":/images/close2.png");
    closeButton->setIcon(icon);
    connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));

    debugButton = new QToolButton();
    QIcon icon2(":/images/debug.png");
    debugButton->setIcon(icon2);
    debugButton->setCheckable(true);
    connect(debugButton,SIGNAL(clicked()),this,SLOT(activateDebug()));

    lay = new QHBoxLayout();
    vlay = new QVBoxLayout();

    lay->addLayout(vlay);

    lay->setSizeConstraint(lay->SetMaximumSize);

    vlay->addWidget(closeButton);
    vlay->addWidget(debugButton);

    lay->setAlignment(vlay,Qt::AlignTop);
    lay->addWidget(window);

    this->setLayout(lay);

    visible = false;
}

CommandLine::~CommandLine()
{
    delete this->window;
    delete this->closeButton;
    delete this->debugButton;
    delete this->vlay;
    delete this->lay;
}

void CommandLine::addLine(QString line,QColor txtColor)
{
    QString toWrite = "<font color=\"" + txtColor.name()
                      + "\">" + line + "</font>";
    window->appendHtml(toWrite);
}

void CommandLine::addDebugLine(QString line)
{
    if(!debugMode)
        return;

    window->appendPlainText(line);
}

void CommandLine::showM()
{
    visible = true;
    parentIde->addItemsToLayout(this);
    this->show();
}

void CommandLine::activateDebug()
{
    if(debugButton->isChecked())
    {
        addLine("=== Debug Mode Activated ===", Qt::darkRed);
        addLine("Please run only on one device, or output will be mixed",
                Qt::darkRed);
        debugMode = true;
    }
    else
        debugMode = false;
}

void CommandLine::clearAll()
{
    window->clear();
}

bool CommandLine::isVisible()
{
    return visible;
}
