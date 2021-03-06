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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <cmath>
#include <vector>

#include "dela.h"
#include "vector.h"

class Primitive : public dela::Scriptable
{
public:
    vec color;
    float mirror;

    Primitive(const vec &color, float mirror = 0.3)
	: color(color), mirror(mirror) {};
    virtual ~Primitive() {};

    virtual float intercept(const Ray &ray) = 0;
    virtual const vec normalAt(vec &point) = 0;

    virtual const vec colorAt(vec & /* point */) {
        return color;
    };

    virtual float getMirror() { return mirror; };
    virtual void setMirror(float mirror) { this->mirror = mirror; };
};

class Sphere : public Primitive
{
public:
    Sphere(const vec &pos, float radius, const vec &color) 
        : Primitive(color), pos(pos), radius(radius) {};

    vec pos;
    float radius;

    virtual float intercept(const Ray &ray) {
      vec e = pos - ray.pos;
      float a = e.dot(ray.dir);
      float f = sqrt(radius * radius - (e.dot(e)) + (a * a));
      return a - f;
    };


    virtual const vec normalAt(vec &point) {
	/*
	vec v = (point - pos).normal(); 
	float r1 = 0.1 * (rand() % 100) / 100.0; 
	float r2 = 0.1 * (rand() % 100) / 100.0;
	float r3 = 0.1 * (rand() % 100) / 100.0;
	return (v + vec(r1, r2, r3)).normal();
	*/

	return (point - pos).normal();
    }
};

class Plane : public Primitive
{
public:
    Plane(const vec &pos, const vec &normal, const vec &color)
        : Primitive(color), pos(pos), normal(normal.normal()) {};

    vec pos;
    vec normal;

    virtual float intercept(const Ray &ray) {
        float d = pos.dot(normal);
        float a = d - ray.pos.dot(normal);
        float b = ray.dir.dot(normal);
        return a / b;
    };


    virtual const vec normalAt(vec & /* point */) {
        return this->normal;
    }

    virtual const vec colorAt(vec &point) {
        vec p = point + pos;
        int a = (int(fabs(p.z < 0 ? p.z - 1 : p.z)) % 2);
        int b = (int(fabs(p.x < 0 ? p.x - 1 : p.x)) % 2);
        return a == b ? vec(0.6, 0.6, 0.6) : vec(1, 1, 1);
    };
};

#endif
