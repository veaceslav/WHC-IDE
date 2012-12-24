#ifndef DIAGRAMDATA_H
#define DIAGRAMDATA_H

#include "diagramitem.h"

class DiagramData : public DiagramItem
{
public:
    DiagramData(const QModelIndex &index, QGraphicsItem *parent = 0);
    enum { Type = UserType + 3 };
    int type() const { return Type; }
};

#endif // DIAGRAMDATA_H
