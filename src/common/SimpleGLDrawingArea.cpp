/*
 * SimpleGLDrawingArea.cpp
 *
 *  Created on: 22 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "SimpleGLDrawingArea.h"

namespace cryo {

namespace viewer {

namespace common {

SimpleGLDrawingArea::SimpleGLDrawingArea() {
}

SimpleGLDrawingArea::~SimpleGLDrawingArea() {
}

void SimpleGLDrawingArea::update() {

}

void SimpleGLDrawingArea::initialise() {
	this->update();

	add_events(
			Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
					| Gdk::VISIBILITY_NOTIFY_MASK);
}

boost::shared_ptr<SimpleGLShape> SimpleGLDrawingArea::add(boost::shared_ptr<SimpleGLShape> shape) {
	shapes[shape->getUUID()] = shape;
	return shape;
}

boost::shared_ptr<SimpleGLShape> SimpleGLDrawingArea::remove(boost::shared_ptr<SimpleGLShape> shape) {
	boost::shared_ptr<SimpleGLShape> found_shape;
	std::map<boost::uuids::uuid, boost::shared_ptr<SimpleGLShape> >::iterator it_found = shapes.find(shape->getUUID());
	if (it_found != shapes.end()) {
		found_shape = it_found->second;
		shapes.erase(it_found);
	}
	return found_shape;
}

void SimpleGLDrawingArea::clear() {
	shapes.clear();
}

void SimpleGLDrawingArea::invalidate() {
	get_window()->invalidate_rect(get_allocation(), false);
}

void SimpleGLDrawingArea::onRealize() {
	// Configure OpenGL-capable visual.
		Glib::RefPtr < Gdk::GL::Config > glconfig;
		// Try double-buffered visual
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
		if (!glconfig) {
			std::cerr << "*** Cannot find the double-buffered visual.\n" << "*** Trying single-buffered visual.\n";

			// Try single-buffered visual
			glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH);
			if (!glconfig) {
				std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
				std::exit(1);
			}
		}
		this->set_gl_capability(glconfig);
		this->initialise();
		this->show();

	Glib::RefPtr < Gdk::GL::Window > glwindow = get_gl_window();
	bool got_window = glwindow->gl_begin(get_gl_context());
	if (got_window == false) {
		std::cout << "StructureGLDrawingArea::drawBundle: " << "ERROR: Could not acquire gl window" << std::endl;
		false;
	} else {
		//TODO			glwindow->gl_end();
	}
}
bool SimpleGLDrawingArea::onConfigureEvent(GdkEventConfigure* event) {
	Glib::RefPtr < Gdk::GL::Window > glwindow = get_gl_window();
	bool got_window = glwindow->gl_begin(get_gl_context());
	if (got_window == false) {
		std::cout << "StructureGLDrawingArea::drawBundle: " << "ERROR: Could not acquire gl window" << std::endl;
		false;
	} else {
		//TODO
		glwindow->gl_end();
	}
	return true;
}
bool SimpleGLDrawingArea::onExposeEvent(GdkEventExpose* event) {
	std::cout << "SimpleGLDrawingArea::onExposeEvent: " << "" << std::endl;
	Glib::RefPtr < Gdk::GL::Window > glwindow = get_gl_window();
	bool got_window = glwindow->gl_begin(get_gl_context());
	if (got_window == false) {
		std::cout << "StructureGLDrawingArea::drawBundle: " << "ERROR: Could not acquire gl window" << std::endl;
		false;
	} else {
		drawShapes();
		glwindow->gl_end();
	}
	return true;
}
bool SimpleGLDrawingArea::on_button_press_event(GdkEventButton* event) {
	std::cout << "SimpleGLDrawingArea::on_button_press_event: " << "" << std::endl;
	// don't block
	return false;
}

bool SimpleGLDrawingArea::on_button_release_event(GdkEventButton* event) {
	std::cout << "SimpleGLDrawingArea::on_button_release_event: " << "" << std::endl;
	return false;
}

bool SimpleGLDrawingArea::on_motion_notify_event(GdkEventMotion* event) {
	float w = this->get_width();
	float h = this->get_height();
	float x = event->x;
	float y = event->y;
	bool redraw = false;
	std::cout << "SimpleGLDrawingArea::on_motion_notify_event: " << "" << std::endl;

	if (redraw == true) {
		this->invalidate();
	}
	// don't block
	return false;
}
void SimpleGLDrawingArea::drawShapes() {
	Glib::RefPtr < Gdk::GL::Window > glwindow = get_gl_window();
	bool got_window = glwindow->gl_begin(get_gl_context());
	if (got_window == false) {
		std::cout << "SimpleGLDrawingArea::drawShapes: " << "ERROR: Could not acquire gl window" << std::endl;
		return;
	} else {
		// forall in shapes
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<SimpleGLShape> >::const_iterator it_shapes = shapes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<SimpleGLShape> >::const_iterator it_shapes_end =
					shapes.end();
			while (it_shapes != it_shapes_end) {
				it_shapes->second->draw(glwindow);
				++it_shapes;
			}
		}
		glwindow->gl_end();
	}
}
}//NAMESPACE

}//NAMESPACE

}//NAMESPACE
