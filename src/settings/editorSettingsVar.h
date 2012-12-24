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

#ifndef EDITORSETTINGSVAR_H
#define EDITORSETTINGSVAR_H
#include "ide.h"
#include <QSettings>

class Ide;
class EditorSettingsVar{
public:
    Ide *parent;
    int tabSize, fontSize;
    bool tabToSpaces;
    QString fontFamily;
    bool endLineSpace;

    explicit EditorSettingsVar(Ide *parent);
    ~EditorSettingsVar();
};

#endif // EDITORSETTINGSVAR_H
