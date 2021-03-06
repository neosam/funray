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
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>

#include "canvas.h"
#include "dela.h"
#include "dela_builtins.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    Canvas canvas;

    if (argc > 1) {
	if (canvas.loadScene(argv[1])) {
	    if ((argc > 2) && QString(argv[2]) == "--autorefresh")
		canvas.setAutoRefresh(true);
	    canvas.show();
	    canvas.render();
	    return app.exec();
	}
    } else {
	std::cout << "Usage: " << argv[0] << " scene-file [--autorefresh]" << std::endl;
    }

    return 0;
}
