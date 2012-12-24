#include "diagramdata.h"

DiagramData::DiagramData(const QModelIndex &index, QGraphicsItem *parent)
    : DiagramItem(index, parent)
{
    font = QFont("Monospace", 10, QFont::Light);

    this->label = QObject::tr("%1 (%2)").arg(name).arg(id);
}
