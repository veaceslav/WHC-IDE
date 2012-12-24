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

#ifndef STATICMETHODS_H
#define STATICMETHODS_H
#include "ide.h"

class QDir;
/**
 * @brief The StaticMethods class contains all methods related  to project
 *        management that do not require a separate class: delete, update etc..
 */
class StaticMethods
{
public:
    StaticMethods();
    ~StaticMethods();

    static void deleteItem(Ide *parent, QModelIndexList indexes);

    static bool removeDirectory(QDir &aDir);

};
#endif // STATICMETHODS_H
