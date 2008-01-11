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

#include <iostream>

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QWidget>


#include "canvas.h"
#include "vector.h"
#include "renderer.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
      scene(),
      renderer(scene, 640, 480)
{
    renderer.setListener(this);
    setMinimumSize(renderer.getWidth(), renderer.getHeight());
    setMaximumSize(renderer.getWidth(), renderer.getHeight());
}

Canvas::~Canvas()
{
}

void Canvas::render()
{
    std::cout << "Start..." << std::endl;
    QTime t;
    t.start();
    renderer.render();
    std::cout << "Finished in " << t.elapsed() << " ms." << std::endl;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    int y1 = rect.y(),
        y2 = y1 + rect.height(),
        x1 = rect.x(),
        x2 = x1 + rect.width();
    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            const vec & pixel = renderer.getPixel(x, y);
            painter.setPen(QColor((int)(pixel.x * 255), 
                                  (int)(pixel.y * 255), 
                                  (int)(pixel.z * 255)));
            painter.drawPoint(x, y);
        }
    }
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
	close();
    else if ((event->key() == Qt::Key_R && event->modifiers() == Qt::ControlModifier)
	     || (event->key() == Qt::Key_F5)
	     || (event->key() == Qt::Key_Return))
	render();
}
