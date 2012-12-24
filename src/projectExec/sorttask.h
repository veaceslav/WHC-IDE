/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
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

#include "ide.h"
#include "diagram/containers.h"
#include <QList>
#include <QPair>

class Node
{
public:

    enum Color {Black, White};
    int diagId;
    QString Name;
    int type;
    int time;
    bool done;

    Color nodeColor;

    QVector<QVector<Node*> > link;

    Node(int conMax,int diagId,QString name, int type)
    {
        link = QVector<QVector<Node*> >(conMax+4);
        this->diagId = diagId;
        this->Name = name;
        this->type = type;
        time = 0;
        nodeColor = Node::White;
        done = false;
    }
};

class SortTasks
{
public:
    SortTasks(Ide* parent, QVector<QPair<ExecNode, ExecNode> > data);
    ~SortTasks();

    /**
     * @brief getExecutionOrder - return a vector with execution order for
     *                            Execute class
     * @return
     */
    QVector<Node*> getExecutionOrder() const
                    { return execOrder; }

private:

    /**
     * @brief dfs -Dfs used for Topological Sort
     * @param nod - node to expand
     * @param time - time when node was discovered
     */
    void dfs(Node* nod, int& time);
    QMap<int,Node*> graph;

    QVector<Node*> execOrder;
};
