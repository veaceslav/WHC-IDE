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
#include "mdidiagram.h"
#include "diagram/diagramwindow.h"
#include <QMdiArea>

MdiDiagram::MdiDiagram(DiagramWindow *diag, QMdiArea *parent,
                       Qt::WindowFlags flags) :
    QMdiSubWindow(parent, flags), mdiParent(parent)
{

    setWidget(diag);
    setAttribute(Qt::WA_DeleteOnClose, false);
    visible = true;

}

void MdiDiagram::addtoMdi()
{
    mdiParent->addSubWindow(this);
    visible = true;
    this->show();
}

void MdiDiagram::closeEvent(QCloseEvent *closeEvent)
{
    Q_UNUSED(closeEvent);
    visible = false;
    mdiParent->removeSubWindow(this);
}
