#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "canvas.h"
#include "vector.h"
#include "raytracer.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
      raytracer(800, 600)
{
    raytracer.setListener(this);
    setMinimumSize(raytracer.getWidth(), raytracer.getHeight());
    setMaximumSize(raytracer.getWidth(), raytracer.getHeight());
}

Canvas::~Canvas()
{
}

void Canvas::raytrace()
{
    std::cout << "Start..." << std::endl;
    raytracer.raytrace();
    std::cout << "Finished!" << std::endl;
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
            const vec & pixel = raytracer.getPixel(x, y);
            painter.setPen(QColor((int)(pixel.x * 255), 
                                  (int)(pixel.y * 255), 
                                  (int)(pixel.z * 255)));
            painter.drawPoint(x, y);
        }
    }
}
