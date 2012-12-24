/* ============================================================
 *
 * This file is a part of WHC IDE Project
 * http://http://whcomputing.wikispaces.com/
 *
 * Date        :
 * Description :
 *
 * Copyright (C) 2012 by Cosmin Mihai <yonutix.mc@gmail.com>
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

#include "settings/editorSettingsVar.h"

EditorSettingsVar::EditorSettingsVar(Ide* parent)
{
    this->parent = parent;
    QSettings sets("WHC","WHC IDE");

    QString line = sets.value("tabSize").toString();
    if(line.toInt() == 0){
        tabSize = 8;
    }
    else{
        tabSize = line.toInt();
    }

    line = sets.value("tabToSpaces").toString();
    if(line.toInt() == 0){
        tabToSpaces = false;
    }
    else{
        tabToSpaces = true;
    }

    line = sets.value("fontFamily").toString();
    if(line.isEmpty()){
        fontFamily = QString("Courier New");
    }else{
        fontFamily = line;
    }

    line = sets.value("fontSize").toString();
    if(fontSize == 0){
        fontSize = 10;
    }
    else{
        fontSize = line.toInt();
    }
    line = sets.value("endLineSpaces").toString();
    if(line.toInt() == 0){
        endLineSpace = false;
    }
    else{
        endLineSpace = true;
    }
}

EditorSettingsVar::~EditorSettingsVar()
{
}
