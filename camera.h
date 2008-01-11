/*funray - yet another raytracer
  Copyright (C) 2008  Christian Zeller (chrizel@gmail.com) and
  Simon Goller (neosam@gmail.com).

  This program is free software; you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published 
  by the Free Software Foundation; either version 3 of the License, 
  or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License along 
  with this program; if not, see <http://www.gnu.org/licenses/>. */

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

class Camera
{
public:
    Camera(const vec &pos,
           const vec &dir, 
           double hlen, 
           double vlen);
    virtual ~Camera();

    vec pos;
    vec dir;
    double hlen;
    double vlen;

    const vec dirVecFor(int x, int y, int width, int height) const;
};

#endif
