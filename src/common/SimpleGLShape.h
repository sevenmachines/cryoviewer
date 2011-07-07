/*
 * SimpleGLShape.h
 *
 *  Created on: 22 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef SIMPLEGLSHAPE_H_
#define SIMPLEGLSHAPE_H_

#include "common/Tagged.h"
#include "spacial/Point.h"

#include <gtkmm/gl/drawingarea.h>
#include <gtkglmm.h>
#include <vector>

namespace cryo {

namespace viewer {

namespace common {

class SimpleGLShape : public cryomesh::common::Tagged {
public:
	enum ShapeType {
		SPHERE, CUBE, LINE, CYLINDER, POINT
	};

	SimpleGLShape(const ShapeType type);
	SimpleGLShape(const ShapeType type, std::vector<cryomesh::spacial::Point> & newpoints);
virtual ~SimpleGLShape();
	void draw(Glib::RefPtr<Gdk::GL::Window> glwindow);

protected:

	void drawPoint(Glib::RefPtr<Gdk::GL::Window> glwindow);
	void drawLine(Glib::RefPtr<Gdk::GL::Window> glwindow);
	void drawSphere(Glib::RefPtr<Gdk::GL::Window> glwindow);

private:
	const ShapeType shapeType;
	std::vector<cryomesh::spacial::Point> points;

};

}

}

}

#endif /* SIMPLEGLSHAPE_H_ */
