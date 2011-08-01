/*
 * SimpleGLShape.cpp
 *
 *  Created on: 22 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "SimpleGLShape.h"
#include <GL/gl.h>

#include <GL/glu.h>

using namespace cryomesh;
using namespace cryomesh::common;
using namespace cryomesh::spacial;

namespace cryo {

namespace viewer {

namespace common {

SimpleGLShape::SimpleGLShape(const ShapeType type) :
	shapeType(type) {
}

SimpleGLShape::SimpleGLShape(const ShapeType type, std::vector<cryomesh::spacial::Point> & newpoints) :
	shapeType(type), points(newpoints) {
}

SimpleGLShape::~SimpleGLShape() {
	// TODO Auto-generated destructor stub
}

void SimpleGLShape::draw(Glib::RefPtr<Gdk::GL::Window> glwindow) {
	int required_points = 0;
	if (shapeType == SimpleGLShape::POINT) {
		required_points = 1;
		if (required_points <= points.size()) {
			drawPoint(glwindow);
		} else {
			std::cout << "SimpleGLShape::draw: " << "Not enough points for Point " << required_points << "/"
					<< points.size() << std::endl;
		}
	} else if (shapeType == SimpleGLShape::LINE) {
		required_points = 2;
		if (required_points <= points.size()) {
			drawLine(glwindow);
		} else {
			std::cout << "SimpleGLShape::draw: " << "Not enough points for Line " << required_points << "/"
					<< points.size() << std::endl;
		}
	} else if (shapeType == SimpleGLShape::SPHERE) {
		required_points = 2;
		if (required_points <= points.size()) {
			drawSphere(glwindow);
		} else {
			std::cout << "SimpleGLShape::draw: " << "Not enough points for Sphere " << required_points << "/"
					<< points.size() << std::endl;
		}
	}

}

void SimpleGLShape::drawPoint(Glib::RefPtr<Gdk::GL::Window> glwindow) {
	std::cout << "SimpleGLShape::drawPoint: " << "TODO" << std::endl;
}
void SimpleGLShape::drawLine(Glib::RefPtr<Gdk::GL::Window> glwindow) {
	std::cout << "SimpleGLShape::drawSphere: " << "TODO" << std::endl;
}
void SimpleGLShape::drawSphere(Glib::RefPtr<Gdk::GL::Window> glwindow) {
	const int SLICES = 20;
	const int STACKS = 20;
	const double RADIUS = points.at(0).getDistance(points.at(1));
	const Point centre = points.at(0);
	{
		glPushMatrix();
		GLUquadric * quad = gluNewQuadric();
		gluQuadricNormals(quad, GLU_SMOOTH);
		glColor3f(0.5, 0.5, 0.5);
		glTranslated(centre.getX(), centre.getY(), centre.getZ());
		gluSphere(quad, RADIUS, SLICES, STACKS);
		gluDeleteQuadric(quad);
		glPopMatrix();
	}
}

}//NAMESPACE

}//NAMESPACE

}//NAMESPACE
