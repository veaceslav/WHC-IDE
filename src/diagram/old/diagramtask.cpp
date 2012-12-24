#include "diagramtask.h"

DiagramTask::DiagramTask(const QModelIndex &index, QGraphicsItem *parent)
    : DiagramItem(index, parent)
{
    font = QFont("times", 12, QFont::Bold, true);

    this->label = QObject::tr("%1 (%2)").arg(name).arg(id);
}
