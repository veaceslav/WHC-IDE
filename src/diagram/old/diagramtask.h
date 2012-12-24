#ifndef DIAGRAMTASK_H
#define DIAGRAMTASK_H

#include "diagramitem.h"

class DiagramTask : public DiagramItem
{
public:
    explicit DiagramTask(const QModelIndex &index, QGraphicsItem *parent = 0);
    enum { Type = UserType + 2 };
    int type() const { return Type; }
};

#endif // DIAGRAMTASK_H
