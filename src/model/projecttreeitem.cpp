#include <QStringList>
#include <QDebug>
#include <QPointF>
#include "projecttreeitem.h"

ProjectTreeItem::ProjectTreeItem(QDomNode nod, ProjectTreeItem *parent)
{
    parentItem = parent;
    node = nod;
    itemName = nod.attributes().namedItem("name").nodeValue();
}

ProjectTreeItem::~ProjectTreeItem()
{
    qDeleteAll(childItems);
}

void ProjectTreeItem::appendChild(ProjectTreeItem* child)
{
    childItems.append(child);
}

ProjectTreeItem* ProjectTreeItem::child(int row)
{
    return childItems.value(row);
}

int ProjectTreeItem::childCount() const
{
    return childItems.count();
}

int ProjectTreeItem::columnCount() const
{
    return 1;
}

QVariant ProjectTreeItem::data(int role) const
{
    QString name = node.nodeName();
    switch (role){
    case Qt::DisplayRole:
    case Qt::EditRole:
        if ((name == "project") ||
                (name == "task") ||
                (name == "file") ||
                (name == "group")){
            return itemName;
        }
        if ((name == "input") ||
                (name == "output")){
            return name;
        }
        if (name == "tasks"){
            return QObject::tr("Tasks");
        }

        if (name == "data"){
            return QObject::tr("Data");
        }
        break;
    case ProjectTreeModel::IdRole:
            return name;
        break;
    case ProjectTreeModel::FileNameRole:{
        QString fileName;
        if (name == "file"){
            QString fold;
            if(node.parentNode().nodeName() == "task")
                fold = "/src/";
            else
                fold = "/data/";
            fileName += fold
                    + node.parentNode().attributes().namedItem("name").nodeValue()+"/"
                    + node.attributes().namedItem("name").nodeValue();
            return fileName;
        }
        break;
    }
    case ProjectTreeModel::PositionRole:
        if ((name == "task") ||
                (name == "group"))
            return QPointF(node.attributes().namedItem("x").nodeValue().toFloat(),
                           node.attributes().namedItem("y").nodeValue().toFloat());
        break;
    }

    return QVariant();

}

bool ProjectTreeItem::setData(const QVariant &value, int role)
{
    if(role == Qt::EditRole)
    {
        if((node.nodeName() == "task")
            || (node.nodeName() == "file")
            || (node.nodeName() == "group"))
        {
            node.attributes().namedItem("name").setNodeValue(value.toString());
            return true;
        }
    }
    return false;
}

ProjectTreeItem* ProjectTreeItem::parent()
{
    return parentItem;
}

int ProjectTreeItem::row() const
{
    if(parentItem)
        return parentItem->childItems.indexOf(const_cast<ProjectTreeItem*>(this));
    else
        return 0;
}

QDomNode ProjectTreeItem::getNode()
{
    return node;
}

bool ProjectTreeItem::searchChildByName(QString name)
{
    if(childItems.isEmpty())
        return false;

    for(int i=0;i<childItems.count();++i)
    {
        if(childItems.at(i)->getName() == name)
            return true;
    }
    return false;
}

void ProjectTreeItem::deleteChild(int poz)
{
    childItems.removeAt(poz);
}
