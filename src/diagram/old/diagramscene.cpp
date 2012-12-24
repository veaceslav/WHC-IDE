#include "diagramscene.h"

#include "diagramtask.h"
#include "diagramdata.h"

#include <QDebug>

DiagramScene::DiagramScene(ProjectTreeModel *model, QObject *parent)
    : QGraphicsScene(parent)
{
    setModel(model);
}

void DiagramScene::setModel(ProjectTreeModel *model)
{
    this->projectModel = model;

    // add the data nodes
    QModelIndex dataIndex = model->dataIndex();
    for (int i = 0; i < model->rowCount(dataIndex); ++i){
        addItem(new DiagramData(model->index(i, 0, dataIndex)));
    }
    // add the task nodes and make the connections
    QModelIndex tasksIndex = model->tasksIndex();
    for (int i = 0; i < model->rowCount(tasksIndex); ++i){
        QModelIndex taskIndex = model->index(i, 0, tasksIndex);
        DiagramTask *taskItem = new DiagramTask(taskIndex);
        addItem(taskItem);
        qDebug() << "Current task:" << taskIndex.data().toString();
        for (int j = 0; j < model->rowCount(taskIndex); ++j){
            QModelIndex childIndex = model->index(j, 0, taskIndex);
            foreach(QGraphicsItem *item, items())
                if(DiagramData *dataItem = qgraphicsitem_cast<DiagramData*>(item))
                    if (dataItem->id == childIndex.data(ProjectTreeModel::IdRole)){
                        if (childIndex.data() == "input"){
                            addItem(new DiagramConnection(dataItem, taskItem));
                            qDebug() << "Adding connection from data" << dataItem->label
                                     << "to task" << taskItem->label;
                        }
                        if (childIndex.data() == "output"){
                            addItem(new DiagramConnection(taskItem, dataItem));
                            qDebug() << "Adding connection from task" << taskItem->label
                                     << "to data" << dataItem->label;
                        }
                    }
        }
    }
    update();
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton) return;

//    DiagramTask *item = NULL;
//    switch (myMode) {
//    case InsertItem:
//        this->itemUpdateOnMove = UPDATE_LIMIT;
//        this->diagSceneID++;
//        item = new DiagramTask(this->diagSceneID, this->project, this->newTaskS, 0);
//        addItem(item);
//        item->final=false;
//        item->initial=false;
//        item->setPos(mouseEvent->scenePos());
//        emit itemInserted(item);
//        break;

//    case InsertLine:
//        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
//        line->setPen(QPen(Qt::black, 2));
//        addItem(line);
//        break;

//    case MoveItem:
//        // select new item, save data for previous
//        this->itemUpdateOnMove=UPDATE_LIMIT;
//        changeItem();
//        break;
//    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

//    DiagramTask* selectedTask = qgraphicsitem_cast<DiagramTask*>(itemAt(mouseEvent->scenePos()));
//    QModelIndex index = selectedTask->index();
//    model->setData(index, mouseEvent->scenePos(), ProjectTreeModel::PositionRole);
//    if (myMode == InsertLine && line != 0)
//    {
//        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
//        line->setLine(newLine);
//    }
//    else if (myMode == MoveItem)
//    {
//        if(this->itemUpdateOnMove>0)
//        {
//            changeItem();
//            this->itemUpdateOnMove--;
//        }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
//    }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if (line != 0 && myMode == InsertLine) {
//        QList<QGraphicsItem *> startItems = items(line->line().p1());
//        if (startItems.count() && startItems.first() == line)
//            startItems.removeFirst();
//        QList<QGraphicsItem *> endItems = items(line->line().p2());
//        if (endItems.count() && endItems.first() == line)
//            endItems.removeFirst();

//        removeItem(line);
//        delete line;
//        //! [11] //! [12]

//        if (startItems.count() > 0 && endItems.count() > 0 &&
//                startItems.first()->type() == DiagramTask::Type &&
//                endItems.first()->type() == DiagramTask::Type &&
//                startItems.first() != endItems.first()) {
//            DiagramTask *startItem =
//                    qgraphicsitem_cast<DiagramTask *>(startItems.first());
//            DiagramTask *endItem =
//                    qgraphicsitem_cast<DiagramTask *>(endItems.first());
//            if((!endItem->initial)&&(!startItem->final))
//            {
//                DiagramConnection *diagramConnection = new DiagramConnection(startItem, endItem);
//                diagramConnection->setZValue(-1000.0);
//                addItem(diagramConnection);
//                if(!updateIO()){
//                    this->removeItem(diagramConnection);
//                    updateIO();
//                }

//            }
//        }

//    }
//    line = 0;

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
