/*
 * ActivityDrawingArea.cpp
 *
 *  Created on: 21-May-2009
 *      Author: niall
 */

#include "ActivityDrawingArea.h"
#include "common/Maths.h"
#include <gtkmm/main.h>
#include <iostream>
#include <algorithm>

namespace cryo {

namespace viewer {

namespace display {

const int ActivityDrawingArea::ACTIVITY_HEIGHT = 100;

const ActivityDrawingArea::DisplayColours ActivityDrawingArea::STANDARD_COLOURS = ActivityDrawingArea::DisplayColours(
		Gdk::Color("red"), Gdk::Color("black"), Gdk::Color("grey"));
const ActivityDrawingArea::DisplayColours ActivityDrawingArea::INACTIVE_COLOURS = ActivityDrawingArea::DisplayColours(
		Gdk::Color("red"), Gdk::Color("grey20"), Gdk::Color("grey"));
const ActivityDrawingArea::DisplayColours ActivityDrawingArea::FIRED_COLOURS = ActivityDrawingArea::DisplayColours(
		Gdk::Color("green"), Gdk::Color("SpringGreen"), Gdk::Color("grey"));
const ActivityDrawingArea::DisplayColours ActivityDrawingArea::PRIMARY_INPUT_COLOURS =
		ActivityDrawingArea::DisplayColours(Gdk::Color("LightSkyBlue"), Gdk::Color("MediumBlue"), Gdk::Color("grey"));
const ActivityDrawingArea::DisplayColours ActivityDrawingArea::PRIMARY_OUTPUT_COLOURS =
		ActivityDrawingArea::DisplayColours(Gdk::Color("Magenta"), Gdk::Color("DarkOrchid"), Gdk::Color("grey"));

ActivityDrawingArea::ActivityDrawingArea() :
	currentColourScheme(ActivityDrawingArea::INACTIVE_COLOURS),
			defaultColourScheme(ActivityDrawingArea::STANDARD_COLOURS) {

	//std::cout << "ActivityDrawingArea::ActivityDrawingArea()" << std::endl;
#ifdef ENABLE_GTK2
	this->signal_expose_event().connect(sigc::mem_fun(*this, &ActivityDrawingArea::on_expose_event));

#else
	this->signal_draw().connect(sigc::mem_fun(*this, &ActivityDrawingArea::on_draw));
#endif
	set_size_request(-1, ActivityDrawingArea::ACTIVITY_HEIGHT);
	this->setActivated(false);
}

ActivityDrawingArea::~ActivityDrawingArea() {
}

void ActivityDrawingArea::setDefaultColourScheme(DisplayColourScheme colour_scheme) {
	switch (colour_scheme) {
	case (INACTIVE_COLOUR_SCHEME): {
		defaultColourScheme = INACTIVE_COLOURS;
		break;
	}
	case (PRIMARY_INPUT_COLOUR_SCHEME): {
		defaultColourScheme = PRIMARY_INPUT_COLOURS;
		break;
	}
	case (PRIMARY_OUTPUT_COLOUR_SCHEME): {
		defaultColourScheme = PRIMARY_OUTPUT_COLOURS;
		break;
	}
	default: {
		defaultColourScheme = STANDARD_COLOURS;
		break;
	}
	}
}
void ActivityDrawingArea::setColourScheme(DisplayColourScheme colour_scheme) {
	switch (colour_scheme) {
	case (DEFAULT_COLOUR_SCHEME): {
		currentColourScheme = defaultColourScheme;
		break;
	}
	case (STANDARD_COLOUR_SCHEME): {
		currentColourScheme = STANDARD_COLOURS;
		break;
	}
	case (INACTIVE_COLOUR_SCHEME): {
		currentColourScheme = INACTIVE_COLOURS;
		break;
	}
	case (FIRED_COLOUR_SCHEME): {
		currentColourScheme.setBackgroundColour(FIRED_COLOURS.getBackgroundColour());
		break;
	}
	case (PRIMARY_INPUT_COLOUR_SCHEME): {
		currentColourScheme = PRIMARY_INPUT_COLOURS;
		break;
	}
	case (PRIMARY_OUTPUT_COLOUR_SCHEME): {
		currentColourScheme = PRIMARY_OUTPUT_COLOURS;
		break;
	}
	default: {
		currentColourScheme = STANDARD_COLOURS;
		break;
	}

	}
}

bool ActivityDrawingArea::isActivated() const {
	return activated;
}

void ActivityDrawingArea::setActivated(bool b) {
	activated = b;
	if (b == true) {
		setColourScheme(ActivityDrawingArea::STANDARD_COLOUR_SCHEME);
		if (this->get_window() != 0) {
			this->invalidateWindow();
			this->update();
		}
	} else {
		setColourScheme(INACTIVE_COLOUR_SCHEME);
	}
}

void ActivityDrawingArea::setLabelText(const std::string & str) {
	labelText = str;
}

void ActivityDrawingArea::setSourceRGB(Cairo::RefPtr<Cairo::Context> cr, const Gdk::Color & col) {
	double r = col.get_red_p();
	double g = col.get_green_p();
	double b = col.get_blue_p();
	//std::cout<<"ActivityDrawingArea::setSourceRGB: "<<"("<<r<<","<<g<<","<<b<<")"<<std::endl;

	cr->set_source_rgb(r, g, b);
}

#ifdef ENABLE_GTK2
bool ActivityDrawingArea::on_expose_event(GdkEventExpose* event) {
	//	std::cout<<"ActivityDrawingArea::on_expose_event: " <<std::endl;
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
#else
	bool ActivityDrawingArea::on_draw(const Cairo::RefPtr< Cairo::Context >& cr) {
#endif
	// background fill
	this->setSourceRGB(cr, currentColourScheme.getBackgroundColour());
	cr->paint();

	//this->update();
	drawPoints(points);
	drawAxis();
	// add text
	drawText();
	return true;
}

void ActivityDrawingArea::drawPoints(std::map<double, double> & ps, const double reference_line) {
	//std::cout<<"ActivityDrawingArea::drawPoints: " <<std::endl;
	// This is where we draw on the window
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	cr->save();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	cr->set_line_width(2.0);
	//  std::cout<<"ActivityDrawingArea::drawPoints: colour: "<<"("<<main_colour[0]<<","<<main_colour[1]<<","<< main_colour[2] <<std::endl;
	this->setSourceRGB(cr, currentColourScheme.getMainColour());

	//scale the drawing in x and y
	double maxy = 0;
	std::map<double, double>::iterator it_ps = ps.begin();
	while (it_ps != ps.end()) {
		if (it_ps->second < 0 && -(it_ps->second) > maxy) {
			maxy = -(it_ps->second);
		} else if (it_ps->second > 0 && (it_ps->second) > maxy) {
			maxy = (it_ps->second);
		}
		++it_ps;
	}

	double scaley = 0.2 * (double) ActivityDrawingArea::ACTIVITY_HEIGHT / maxy;
	double scalex = 10;

	it_ps = ps.begin();
	if (it_ps != ps.end()) {
		cr->move_to(scalex * it_ps->first, (0.5 * height) - (scaley * it_ps->second));
	} else {
		cr->move_to(0, height / 2);
	}
	while (it_ps != ps.end()) {
		//	std::cout<<"ActivityDrawingArea::drawPoints: " << it_ps->first <<","<<-it_ps->second<<std::endl;
		cr->line_to((scalex * it_ps->first), (0.5 * height) - (scaley * it_ps->second));
		++it_ps;
	}
	cr->stroke();

	// draw reference line if not zero
	if (reference_line>0.00001 || reference_line < -0.00001) {
		Gdk::Color temp_colour(currentColourScheme.getMainColour());
		this->setSourceRGB(cr, Gdk::Color("tomato"));
		cr->set_line_width(1.0);
		const std::vector<double> dashed= { 1.0 };
		cr->set_dash(dashed, 1);
		double scaled_reference_line = (0.5 * height) - (scaley * reference_line);
		double neg_scaled_reference_line = (0.5 * height) + (scaley * reference_line);
		cr->move_to(0, scaled_reference_line);
		cr->line_to(width, scaled_reference_line);
		cr->move_to(0, neg_scaled_reference_line);
			cr->line_to(width, neg_scaled_reference_line);
		cr->stroke();
	}
	cr->restore();
}

void ActivityDrawingArea::drawText() {

	// This is where we draw on the window
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	cr->save();
	cr->set_line_width(2.0);
	this->setSourceRGB(cr, currentColourScheme.getAxisColour());
	cr->set_font_size(18);
	cr->move_to(0, 20);

	cr->restore();
}

void ActivityDrawingArea::drawAxis() {
	// draw a reference axis and pod info
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	//draw axis
	cr->save();
	cr->set_line_width(2.0);
	this->setSourceRGB(cr, currentColourScheme.getAxisColour());
	cr->set_font_size(12);

	cr->move_to(0, height / 2);
	cr->line_to(width, height / 2);

	cr->move_to(190, 25 + height / 2);
	cr->show_text("20");
	cr->move_to(390, 25 + height / 2);
	cr->show_text("40");
	cr->move_to(590, 25 + height / 2);
	cr->show_text("60");
	cr->move_to(790, 25 + height / 2);
	cr->show_text("80");

	cr->stroke();
	cr->restore();
}

void ActivityDrawingArea::showDrawingArea(bool b) {
	enable_drawing_area = b;
	if (enable_drawing_area) {
		this->show();
	} else {
		this->hide();
	}
}

void ActivityDrawingArea::invalidateWindow() {
	Gdk::Rectangle lRect = this->get_allocation();
	this->get_window()->invalidate_rect(Gdk::Rectangle(0, 0, lRect.get_width(), lRect.get_height()), false);
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
