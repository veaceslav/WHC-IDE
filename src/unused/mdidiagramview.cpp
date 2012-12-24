#include "mdidiagramview.h"
#include <QVBoxLayout>
#include <QDebug>
#include "ide.h"

MdiDiagramView::MdiDiagramView(QDomDocument* proj, Ide *parent) :
    QWidget(parent)
{

    window = new DiagramWindow(proj,parent);

    QVBoxLayout* lay = new QVBoxLayout();
    lay->addWidget(window);
    this->setLayout(lay);
}

MdiDiagramView::~MdiDiagramView()
{
}
