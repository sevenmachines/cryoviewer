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
	node(nd), isPrimaryInput(false), isPrimaryOutput(false) {
	if (node->isPrimaryInputAttachedNode() == true) {
		this->setAsPrimaryInput();
	}
	if (node->isPrimaryOutputAttachedNode() == true) {
		this->setAsPrimaryOutput();
	}
}

NodeActivityDrawingArea::~NodeActivityDrawingArea() {

}

void NodeActivityDrawingArea::setAsPrimaryInput() {
	this->isPrimaryInput = true;
	this->setDefaultColourScheme(ActivityDrawingArea::PRIMARY_INPUT_COLOUR_SCHEME);
}
void NodeActivityDrawingArea::setAsPrimaryOutput() {
	isPrimaryOutput = true;
	this->setDefaultColourScheme(ActivityDrawingArea::PRIMARY_OUTPUT_COLOUR_SCHEME);

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
			//std::cout << "NodeActivityDrawingArea::update: " << i << " < " << current_cycle + MAX_FUTURE_TICKS
			//<< " activity: " << points[x] << std::endl;
			++x;
		}
#ifdef NODEACTIVITYDRAWINGAREA_DEBUG
		node->setDebug(true);
		std::cout << "NodeActivityDrawingArea::update: " << "Node: " << *node << std::endl;
		node->setDebug(false);
#endif
		//set colour
		if (node->isTriggered() == true) {
			this->setColourScheme(FIRED_COLOUR_SCHEME);
		} else {
			this->setColourScheme(DEFAULT_COLOUR_SCHEME);
		}

	}
	this->invalidateWindow();

}

bool NodeActivityDrawingArea::on_expose_event(GdkEventExpose* event) {
	//	std::cout<<"ActivityDrawingArea::on_expose_event: " <<std::endl;
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	// background fill
	this->setSourceRGB(cr, currentColourScheme.getBackgroundColour());
	cr->paint();

	//this->update();
	drawPoints(points, node->getActivityThreshold());
	drawAxis();
	// add text
	drawText();
	return true;
}

void NodeActivityDrawingArea::drawText() {
	ActivityDrawingArea::drawText();
	// This is where we draw on the window
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	cr->save();
	cr->set_line_width(2.0);
	cr->set_source_rgb(1, 1, 1);
	cr->set_font_size(12);

	{
		cr->move_to(width / 5, 20);
		std::stringstream ss;
		ss << "ID";
		if (this->isPrimaryInput == true) {
			ss << " (PIN)";
		}
		if (this->isPrimaryOutput == true) {
			ss << " (POUT)";
		}
		ss<< ": " <<node->getUUIDString();
		cr->show_text(ss.str());
	}

	{
		std::stringstream ss;
		cr->move_to(width / 5, 40);
		int impulse_count = node->getImpulses().getSize();
		int inputs_count = node->getConnector().getInputs().size();
		int outputs_count = node->getConnector().getOutputs().size();
		double threshold = node->getActivityThreshold();

		ss << "Impulses: " << impulse_count;
		ss << " Inputs: " << inputs_count;
		ss << " Outputs: " << outputs_count;
		ss << " Threshold: " << threshold;
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
