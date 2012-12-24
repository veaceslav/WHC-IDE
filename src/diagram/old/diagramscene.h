#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QString>

#include "model/projecttreemodel.h"
#include "diagramconnection.h"

class DiagramItem;

enum Mode { InsertItem, InsertLine, MoveItem };

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    DiagramScene(ProjectTreeModel *projectModel, QObject *parent = 0);
    void setModel(ProjectTreeModel *projectModel);
    ProjectTreeModel *model() { return projectModel; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

public slots:

signals:

private:
    ProjectTreeModel *projectModel;
};

#endif // DIAGRAMSCENE_H
