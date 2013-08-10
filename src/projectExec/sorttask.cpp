/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description : Topological sort implementation, provides the execution order
 *
 * Copyright (C) 2012 by Veaceslav Munteanu <slavuttici at gmail dot com>
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

#include "sorttask.h"
#include <QDebug>
#include <algorithm>


SortTasks::SortTasks(Ide *parent, QVector<QPair<ExecNode, ExecNode> > data)
{
    Q_UNUSED(parent);

    hasCycle = false;

    for(int i = 0; i < data.count(); i++)
    {
        QPair<ExecNode,ExecNode> tmp = data.at(i);
        Node *nod1, *nod2;
        if(!graph.contains(tmp.first.diagId))
        {
            nod1 = new Node(tmp.first.maxCon, tmp.first.diagId,
                                 tmp.first.name, tmp.first.type);
            graph[tmp.first.diagId] = nod1;
        }
        else
        {
            nod1 = graph.value(tmp.first.diagId);
        }


        if(!graph.contains(tmp.second.diagId))
        {
            nod2 = new Node(tmp.second.maxCon, tmp.second.diagId,
                                 tmp.second.name, tmp.second.type);
            graph[tmp.second.diagId] = nod2;
        }
        else
        {
            nod2 = graph.value(tmp.second.diagId);
        }

        nod1->link[tmp.first.conId].append(nod2);
        nod2->link[tmp.second.conId].append(nod1);
    }

    computeExecOrder();
}

void SortTasks::computeExecOrder()
{
    while(!graph.isEmpty())
    {
        bool removed = false;
        execOrder << QVector<Node *>();
        QVector<Node *> last = execOrder.last();

        for(QMap<int, Node *>::Iterator i = graph.begin(); i != graph.end();)
            if(i.value()->dependencies() == 0)
            {
                if(i.value()->type == 0)
                    last << i.value();
                i = graph.erase(i);
                removed = true;
            }
            else
            {
                i++;
            }

        for(int i = 0; i < last.size(); i++)
        {
            int outIndex = last[i]->link.size() - 1;
            for(int j = 0; j < last[i]->link[outIndex].size(); j++)
                last[i]->removeEdge(j);
        }

        if(!removed)
        {
            hasCycle = true;
            break;
        }
    }
}

Node::Node(int conMax,int diagId,QString name, int type)
{
    link = QVector<QVector<Node*> >(conMax);
    this->diagId = diagId;
    this->Name = name;
    this->type = type;
    done = false;
}

int Node::dependencies() const
{
    int dependNo = 0;

    for(int i = 0; i < link.size() - 1; i++)
        for(int j = 0; j < link[i].size(); j++)
            if(link[i][j]->type != 1 || link[i][j]->dependencies() != 0)
                dependNo++;

    return dependNo;
}

void Node::removeEdge(int pos)
{
    if(pos < 0 || pos >= link[link.size() - 1].size())
        return;

    Node *other = link[link.size() - 1][pos];

    for(int i = 0; i < other->link.size() - 1; i++)
        for(QVector<Node *>::Iterator j = other->link[i].begin();
            j != other->link[i].end();)
            if(*j == this)
                j = other->link[i].erase(j);
            else
                j++;
}
