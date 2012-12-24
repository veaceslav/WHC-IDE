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

#ifndef CONTAINERS_H
#define CONTAINERS_H

class QString;
/**
 * @brief The ExeNode class holds all data for execution graph. Usually it will
 *        be pairs of this class, and one pair will be an arrow with information
 *        about source and destination;
 */
class ExecNode
{
public:

    /** Task/Data name **/
    QString name;
    /** Connector id **/
    int conId;
    /** number of tasks connectors **/
    int maxCon;
    /** Type - Task or Data **/
    int type;
    /** Unique diagram id, used to identify if on diagam  are multiple tasks
     *  with the same name
     */
    int diagId;

    ExecNode()
    {
        conId   = 0;
        maxCon  = 0;
        type    = 0;
        diagId  = 0;
    }
};
/**
 * @brief The DiagramNode class holds data related to Task/Data node
 *        it's used to avoid big amount of arguments.
 */
class DiagramNode
{
public:
    /** Task/Data name **/
    QString name;
    /** number of task/data inputs, note that only task have multiple inputs **/
    int inputs;
    /** number of task/data outputs, deprecated, all task/data have 1 output **/
    int outputs;
    /** task/data id from .whc project file **/
    int id;

    DiagramNode()
    {
        inputs  = 1;
        outputs = 1;
        id      = -1;
    }

    bool operator== (const DiagramNode& other) const
    {
        return id == other.id;
    }
};
#endif // CONTAINERS_H
