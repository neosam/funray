/*funray - yet another raytracer
opyright (C) 2008  Christian Zeller (chrizel@gmail.com) and
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

#include <QByteArray>
#include <QDebug>

#include <cmath>
#include <iostream>

#include "dela.h"
#include "dela_builtins.h"

using namespace dela;

static Scriptable *plus(Engine *e, List *params)
{
    float result = 0;
    for (List::iterator it = params->begin(); it != params->end(); it++)
	result += ensureType<Number>(*it)->value;
    return e->autorelease(new Number(result));
}

static Scriptable *minus(Engine *e, List *params)
{
    int size = params->size();

    if (size > 1) {
	float result = ensureType<Number>(params->first())->value;
	for (List::iterator it = params->begin() + 1; it != params->end(); it++) {
	    result -= ensureType<Number>(*it)->value;
	}
	return e->autorelease(new Number(result));
    } else if (size == 1) {
	return e->autorelease(new Number(-ensureType<Number>(params->first())->value));
    }

    return 0;
}

static Scriptable *multiply(Engine *e, List *params)
{
    int size = params->size();

    if (size > 0) {
	float result = ensureType<Number>(params->first())->value;
	for (List::iterator it = params->begin() + 1; it != params->end(); it++) {
	    result *= ensureType<Number>(*it)->value;
	}
	return e->autorelease(new Number(result));
    }

    return 0;
}

static Scriptable *divide(Engine *e, List *params)
{
    int size = params->size();

    if (size > 0) {
	float result = ensureType<Number>(params->first())->value;
	for (List::iterator it = params->begin() + 1; it != params->end(); it++) {
	    result /= ensureType<Number>(*it)->value;
	}
	return e->autorelease(new Number(result));
    }

    return 0;
}

static Scriptable *display(Engine * /* e */, List *params)
{
    for (List::iterator it = params->begin(); it != params->end(); it++) {
	std::cout << toString(*it).constData() << " ";
    }

    std::cout << std::endl;
    return 0;
}

static Scriptable *list(Engine * /* e */, List *params)
{
    return params;
}

static Scriptable *set(Engine *e, List *params)
{
    int size = params->size();

    if (size == 0 || (size % 2 == 1)) {
	qDebug() << "dela::builtins::set error: set parameter count zero or not even";
	exit(1);
    }

    Scriptable *name = 0;
    Scriptable *value = 0;
    for (List::iterator it = params->begin(); it != params->end(); it += 2) {
	name  = e->eval(*it);
	value = e->eval(*(it + 1));
	e->setVariable(ensureType<String>(name)->value.constData(), value);
    }

    return value;
}

static Scriptable *unset(Engine *e, List *params)
{
    if (params->size() == 1) {
	e->unsetVariable(ensureType<String>(params->at(0))->value.constData());
	return 0;
    }

    qDebug() << "dela::builtins::unset error: unset accepts only one parameter";
    exit(1);
    return 0;
}

static Scriptable *for_loop(Engine *e, List *params)
{
    // For has the following syntax:
    // (for (i 0 10)
    //    code-lines ...)

    // Check params length...
    if (!params->size()) {
	qDebug() << "dela::builtins:for error: for-loop is empty :-(";
	exit(1);
    }

    // Get first parameter-list with variable name and from to values:
    List *list  = ensureType<List>(params->at(0));
    if (list->size() < 3) {
	qDebug() << "dela::builtins:for error: for-loop parameters are wrong, give me 3";
	exit(1);
    }

    // Read loop parameters...
    QByteArray var = ensureType<String>(e->eval(list->at(0)))->value;
    float from = ensureType<Number>(e->eval(list->at(1)))->value;
    float to   = ensureType<Number>(e->eval(list->at(2)))->value;
    float step = (list->size() > 3) ? ensureType<Number>(e->eval(list->at(3)))->value : 1.0;

    // Loop...
    Number *n = e->autorelease(new Number(from));
    e->setVariable(var, n);
    Scriptable *result = 0;
    while (n->value <= to) {
	// Evaluate code...
	for (List::iterator it = params->begin() + 1; it != params->end(); it++)
	    result = e->eval(*it);
	n->value += step;
    }

    return result;
}

static Scriptable *begin(Engine * /* e */, List *params)
{
    if (!params->empty())
	return params->last();
    return 0;
}

static Scriptable *sin(Engine *e, List *params)
{
    if (params->size() != 1) {
	qDebug() << "dela::sin error: wrong number of arguments";
	exit(1);
    }

    float x = sin(ensureType<Number>(params->at(0))->value);
    return e->autorelease(new Number(x));
}

static Scriptable *cos(Engine *e, List *params)
{
    if (params->size() != 1) {
	qDebug() << "dela::cos error: wrong number of arguments";
	exit(1);
    }

    float x = cos(ensureType<Number>(params->at(0))->value);
    return e->autorelease(new Number(x));
}

void dela::addBuiltins(Engine *e) 
{
    e->addFunction("+",       &plus);
    e->addFunction("-",       &minus);
    e->addFunction("*",       &multiply);
    e->addFunction("/",       &divide);
    e->addFunction("sin",     &sin);
    e->addFunction("cos",     &cos);

    e->addFunction("display", &display);
    e->addFunction("begin",   &begin);
    e->addFunction("list",    &list);
    e->addFunction("unset",   &unset);

    e->addMacro("set",        &set);
    e->addMacro("for",        &for_loop);
}    
