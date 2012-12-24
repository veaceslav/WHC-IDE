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

#include "clustersettings.h"

// new cluster added
ClusterSettings::ClusterSettings()
{
  //   this->name = "whc_cluster";
  //   this->username = "student";
  //  this->password = "student";
  //  this->sentinelDir = "/home/student/whc/projects";
  //  this->serverDir = "/home/student/whc/projects";
  //  this->serverIP = "10.38.229.223";
  //  this->sentinelIP = "10.38.220.220";

    this->name = "whc_cluster";
    this->username = "student";
    this->password = "student";
    this->sentinelDir = "/home/student/projects/";
    this->serverDir = "/home/lupescu/whc/projects/";
    this->serverIP = "10.38.229.223";
    this->sentinelIP = "10.38.199.77";
}

