/*
 * NodeActivityDrawingArea.cpp
 *
 *  Created on: 01-Jun-2009
 *      Author: niall
 */

#include "NodeActivityDrawingArea.h"
#include "common/TimeKeeper.h"
#include <sstream>
#include <gmpxx.h>

using namespace cryomesh::components;
using namespace cryomesh::common;

namespace cryo {
namespace viewer {
namespace display {

const int NodeActivityDrawingArea::MAX_FUTURE_TICKS = 100;
NodeActivityDrawingArea::NodeActivityDrawingArea(const boost::shared_ptr<cryomesh::components::Node> & nd) :
	node(nd) {
}

NodeActivityDrawingArea::~NodeActivityDrawingArea() {

}
void NodeActivityDrawingArea::update() {
	this->showDrawingArea(this->isActivated());
	if (this->isActivated() == true) {
		points.clear();

		// get its activity from this tick to MAX_FUTURE_TICKS
		Cycle current_cycle = TimeKeeper::getTimeKeeper().getCycle();
		int x = 0;
		for (Cycle i = current_cycle; i < current_cycle + MAX_FUTURE_TICKS; i++) {
			points[x] = node->getActivity(i);
			std::cout << "NodeActivityDrawingArea::update: " << i << " < " << current_cycle + MAX_FUTURE_TICKS
					<< " activity: " << points[x] << std::endl;
			++x;
		}
		node->setDebug(true);
		std::cout << "NodeActivityDrawingArea::update: " << "Node: " << *node << std::endl;
		node->setDebug(false);

		//set colour
		if (node->isTriggered() == true) {
			colour = fired_colour;
		} else {
			colour = main_colour;
		}

	}
	this->invalidateWindow();

}

void NodeActivityDrawingArea::drawText() {
	ActivityDrawingArea::drawText();
	// This is where we draw on the window
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	cr->save();
	cr->set_line_width(2.0);
	cr->set_source_rgb(1, 1, 1);
	cr->set_font_size(18);

	{
		cr->move_to(width / 2, 20);
		std::stringstream ss;
		ss << "ID: " << node->getUUIDString();
		cr->show_text(ss.str());
	}

	{
		std::stringstream ss;
		cr->move_to(width / 2, 40);
		// 	ss<<"strength: "<<fibril->getMutator().getStrength()
		//    	<<" compression: "<<fibril->getMutator().getCompression()
		//  	<<" delay: " <<fibril->getMutator().getDelay();
		cr->show_text(ss.str());
	}
	cr->restore();
}

void NodeActivityDrawingArea::drawAxis() {

	ActivityDrawingArea::drawAxis();
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
