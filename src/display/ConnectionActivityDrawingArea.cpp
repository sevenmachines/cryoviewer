/*
 * ConnectionActivityDrawingArea.cpp
 *
 *  Created on: 3 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ConnectionActivityDrawingArea.h"
#include "common/Containers.h"
#include "common/TimeKeeper.h"

using namespace cryomesh::common;
using namespace cryomesh::components;

namespace cryo {

namespace viewer {

namespace display {

const int ConnectionActivityDrawingArea::MAX_FUTURE_TICKS = 100;
ConnectionActivityDrawingArea::ConnectionActivityDrawingArea(
		const boost::shared_ptr<cryomesh::components::Connection> & obj) :
	connection(obj), isPrimaryInput(false), isPrimaryOutput(false) {
	if (connection->isPrimaryInputConnection() == true) {
		this->setAsPrimaryInput();
	} else if (connection->isPrimaryOutputConnection() == true) {
		this->setAsPrimaryOutput();
	}
}

ConnectionActivityDrawingArea::~ConnectionActivityDrawingArea() {

}

void ConnectionActivityDrawingArea::setAsPrimaryInput() {
	std::cout << "ConnectionActivityDrawingArea::setAsPrimaryInput: " << "" << std::endl;
	this->isPrimaryInput = true;
	this->setDefaultColourScheme(ActivityDrawingArea::PRIMARY_INPUT_COLOUR_SCHEME);
}
void ConnectionActivityDrawingArea::setAsPrimaryOutput() {
	this->isPrimaryOutput = true;
	std::cout << "ConnectionActivityDrawingArea::setAsPrimaryOutput: " << "" << std::endl;
	this->setDefaultColourScheme(ActivityDrawingArea::PRIMARY_OUTPUT_COLOUR_SCHEME);

}

void ConnectionActivityDrawingArea::update() {
	this->showDrawingArea(this->isActivated());
	if (this->isActivated() == true) {

		multiPoints.clear();

		const std::map<boost::uuids::uuid, boost::shared_ptr<Impulse> > & all_impulses =
				connection->getImpulses().getCollection();
		// forall in all_impulses
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<Impulse> >::const_iterator it_all_impulses =
					all_impulses.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Impulse> >::const_iterator it_all_impulses_end =
					all_impulses.end();
			while (it_all_impulses != it_all_impulses_end) {
				const Impulse & imp = *(it_all_impulses->second);
				double imp_x = imp.getActivityTimer()->getDelay();
				double imp_y = imp.getMaximum();
				std::cout << "ConnectionActivityDrawingArea::update: " << "Impulse: " << imp_x << ", " << imp_y
						<< std::endl;
				std::map<double, std::vector<double> >::iterator found_point = multiPoints.find(imp_x);
				if (found_point != multiPoints.end()) {
					found_point->second.push_back(imp_y);
				} else {
					std::vector<double> temp_vec;
					temp_vec.push_back(imp_y);
					multiPoints[imp_x] = temp_vec;
				}
				++it_all_impulses;
			}
		}

		//set colour
	}
	this->invalidateWindow();

}

void ConnectionActivityDrawingArea::drawText() {
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
		ss << ": " << connection->getUUIDString();
		cr->show_text(ss.str());
	}

	{
		std::stringstream ss;
		cr->move_to(width / 5, 40);
		ss << "Impulses: " << connection->getImpulses().getSize();
		//    	<<" compression: "<<fibril->getMutator().getCompression()
		//  	<<" delay: " <<fibril->getMutator().getDelay();
		cr->show_text(ss.str());
	}
	cr->restore();
}

void ConnectionActivityDrawingArea::drawAxis() {

	// draw a reference axis and pod info
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	double max_delay = connection->getActivityTimer()->getStartingDelay();

	//draw axis
	cr->save();
	cr->set_line_width(2.0);
	this->setSourceRGB(cr, currentColourScheme.getAxisColour());
	cr->set_font_size(12);

	cr->move_to(0, height / 2);
	cr->line_to(width, height / 2);

	{
		std::stringstream ss_temp;
		ss_temp << max_delay / 2;
		cr->move_to(-40 + width / 2, 25 + height / 2);
		cr->show_text(ss_temp.str());
	}
	{
		std::stringstream ss_temp;
		ss_temp << max_delay;
		cr->move_to(width - 40, 25 + height / 2);
		cr->show_text(ss_temp.str());
	}

	cr->stroke();
	cr->restore();
}

void ConnectionActivityDrawingArea::drawBarPoints() {
	// forall in multiPoints
	{
		std::map<double, std::vector<double> >::iterator it_multiPoints = multiPoints.begin();
		const std::map<double, std::vector<double> >::const_iterator it_multiPoints_end = multiPoints.end();
		while (it_multiPoints != it_multiPoints_end) {
			double xval = it_multiPoints->first;
			// sort into descending order
			std::sort(it_multiPoints->second.begin(), it_multiPoints->second.end());
			// forall in bar_points
			{
				int count = 0;
				std::vector<double>::const_reverse_iterator it_bar_points = it_multiPoints->second.rbegin();
				const std::vector<double>::const_reverse_iterator it_bar_points_end = it_multiPoints->second.rend();
				while (it_bar_points != it_bar_points_end) {

					//increase colours
					drawBar(xval, *it_bar_points, 0, STANDARD_COLOURS.getMainColour());
					++count;
					++it_bar_points;
				}
			}
			++it_multiPoints;
		}
	}

}

void ConnectionActivityDrawingArea::drawBar(const double x, const double y, const int overlay_count,
		const Gdk::Color & colour) {
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	cr->save();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	cr->set_line_width(2.0);
	//  std::cout<<"ActivityDrawingArea::drawPoints: colour: "<<"("<<main_colour[0]<<","<<main_colour[1]<<","<< main_colour[2] <<std::endl;
	double r = colour.get_red_p();
	double g = colour.get_green_p();
	double b = colour.get_blue_p();
	double step = 0.1;
	//std::cout<<"ActivityDrawingArea::setSourceRGB: "<<"("<<r<<","<<g<<","<<b<<")"<<std::endl;
	for (int i = overlay_count; i > 0; i--) {
		if (r < 1) {
			r += step;
		} else if (g < 1) {
			g += step;
		} else if (b < 1) {
			b += step;
		}
	}
	cr->set_source_rgb(r, g, b);

	double scalex = width / connection->getActivityTimer()->getStartingDelay();

	//scale the drawing in x and y
	cr->move_to(x * scalex, height / 2);
	cr->line_to(x * scalex, y);
	//std::cout << "ConnectionActivityDrawingArea::drawBar: " << "(" << x * scalex << ", " << height / 2 << ")"
	//		<< std::endl;
	cr->stroke();
	cr->restore();
}
#ifdef ENABLE_GTK2
bool ConnectionActivityDrawingArea::on_expose_event(GdkEventExpose* event) {
	//	std::cout<<"ActivityDrawingArea::on_expose_event: " <<std::endl;
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
#else
		 bool ConnectionActivityDrawingArea::on_draw(const Cairo::RefPtr< Cairo::Context >& cr){
#endif
	// background fill
	this->setColourScheme(DEFAULT_COLOUR_SCHEME);
	this->setSourceRGB(cr, currentColourScheme.getBackgroundColour());
	cr->paint();

	this->drawBarPoints();

	drawAxis();
	// add text
	drawText();
	return true;
}

}

}

}
