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

    int diagId;
    QString Name;
    int type;
    bool done;

    QVector<QVector<Node*> > link;

    Node(int conMax,int diagId,QString name, int type);

    int dependencies() const;
    void removeEdge(int pos);
};

class SortTasks
{
public:
    SortTasks(Ide *parent, QVector<QPair<ExecNode, ExecNode> > data);

    /**
     * @brief getExecutionOrder - returns a vector with execution order for
     *                            Execute class
     * @return                  - returns true if the graph has a cycle,
     *                            false otherwise
     */
    QVector<QVector<Node*> > getExecutionOrder() const
                    { return execOrder; }

    /**
     * @brief containsCycle -
     * @return
     */
    bool containsCycle() const
                    { return hasCycle; }

private:

    bool hasCycle;
    QMap<int, Node *> graph;
    QVector<QVector<Node *> > execOrder;

    void computeExecOrder();
};
