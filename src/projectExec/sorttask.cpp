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

    int time = 0, foundCycle;

    for(QMap<int, Node*>::Iterator it = graph.begin(); it != graph.end(); ++it)
    {
        if(it.value()->nodeColor == Node::White)
            foundCycle = dfs(it.value(), time);
        if(foundCycle)
        {
            hasCycle = true;
            break;
        }
    }

    if(!hasCycle)
        std::reverse(execOrder.begin(), execOrder.end());
}

SortTasks::~SortTasks()
{
    for(int i = 0; i < execOrder.size(); i++)
        delete execOrder.at(i);
}

int SortTasks::dfs(Node *nod, int &time)
{
    int foundCycle = 0;

    nod->nodeColor = Node::Grey;
    time++;

    int outIndex = nod->link.size() - 1;

    QVector<Node*> lst = nod->link[outIndex];

    for(int i = 0; i < lst.size(); i++)
    {
        if(lst.at(i)->nodeColor == Node::White)
            foundCycle = dfs(lst.at(i), time);
        else if(lst.at(i)->nodeColor == Node::Grey)
            foundCycle = 1;
        if(foundCycle)
            break;
    }

    nod->nodeColor = Node::Black;
    time++;

    nod->time = time;
    execOrder.append(nod);

    return foundCycle;
}
