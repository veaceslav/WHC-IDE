#include "mdidiagram.h"
#include "diagram/diagramwindow.h"
#include <QMdiArea>

MdiDiagram::MdiDiagram(DiagramWindow *diag,QMdiArea *parent,
                       Qt::WindowFlags flags) :
    QMdiSubWindow(parent,flags),mdiParent(parent)
{

    setWidget(diag);
    setAttribute(Qt::WA_DeleteOnClose,false);
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
