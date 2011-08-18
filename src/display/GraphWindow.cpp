/*
 * GraphWindow.cpp
 *
 *  Created on: 17 Aug 2011
 *      Author: niall
 */

#include "GraphWindow.h"
#include "common/Maths.h"

using namespace cryomesh::spacial;
using namespace cryomesh::common;

namespace cryo {
namespace viewer {
namespace display {

const int GraphWindow::GRAPH_PROPERTIES = SCALE_X | SCALE_Y;
const int GraphWindow::GRAPH_SIZE = 10;

GraphWindow::GraphWindow() :
		graphProperties(GRAPH_PROPERTIES) {
	this->clear();
	this->setSize(GRAPH_SIZE);
	this->set_size_request(GRAPH_SIZE, 50);
}

GraphWindow::~GraphWindow() {
}

const std::list<cryomesh::spacial::Point> & GraphWindow::getPoints() const {
	return points;
}

void GraphWindow::setSize(const int sz) {
	graphSize = sz;
}

#ifdef ENABLE_GTK2
bool GraphWindow::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
#else
bool GraphWindow::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
#endif
	std::cout << "GraphWindow::on_draw: " << "" << std::endl;
	// background fill
	this->setSourceRGB(cr, Gdk::Color("black"));
	cr->paint();
	drawGraph(cr);
	drawFramework(cr);
	drawText(cr);
	return true;
}

void GraphWindow::invalidateWindow() {
	std::cout << "GraphWindow::invalidateWindow: " << "" << std::endl;
	// force our program to redraw the entire clock.
	Glib::RefPtr<Gdk::Window> win = this->get_window();
	if (win) {
		Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
}

void GraphWindow::drawFramework(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = get_allocation();
	const int WIDTH = allocation.get_width();
	const int HEIGHT = allocation.get_height();

	//draw axis
	cr->save();
	cr->set_line_width(2.0);
	this->setSourceRGB(cr, Gdk::Color("grey"));
	cr->set_font_size(12);

	cr->move_to(0, HEIGHT / 2);
	cr->line_to(WIDTH, HEIGHT / 2);

	cr->move_to(190, 25 + HEIGHT / 2);
	cr->show_text("20");
	cr->move_to(390, 25 + HEIGHT / 2);
	cr->show_text("40");
	cr->move_to(590, 25 + HEIGHT / 2);
	cr->show_text("60");
	cr->move_to(790, 25 + HEIGHT / 2);
	cr->show_text("80");

	cr->stroke();
	cr->restore();
}

void GraphWindow::drawGraph(const Cairo::RefPtr<Cairo::Context>& cr) {
	std::cout << "GraphWindow::drawGraph: " << "" << std::endl;
	cr->save();
	Gtk::Allocation allocation = get_allocation();
	const int WIDTH = allocation.get_width();
	const int HEIGHT = allocation.get_height();

	cr->set_line_width(2.0);
	this->setSourceRGB(cr, Gdk::Color("white"));

	//scale the drawing in x and y
	double maxx = 0;
	double minx = 0;
	double maxy = 0;
	double miny = 0;
	//double diffx = 0;
	//double diffy = 0;
	std::list<cryomesh::spacial::Point>::const_iterator it_ps = points.begin();
	while (it_ps != points.end()) {
		double x_pt = it_ps->getX();
		double y_pt = it_ps->getY();

		if (x_pt > maxx) {
			maxx = x_pt;
		} else if (x_pt < minx) {
			minx = x_pt;
		}

		if (y_pt > maxy) {
			maxy = y_pt;
		} else if (y_pt < miny) {
			miny = y_pt;
		}
		++it_ps;
	}

	//diffx = maxx-minx;
	//diffy = maxy - miny;

	double scaley;
	if (this->graphProperties & GraphProperties::SCALE_Y) {
		double DELTA = 0.01;
		double abs_miny = Maths::absolute<double>(miny);
		double abs_maxy = Maths::absolute<double>(maxy);
		double max_abs ;
		if(abs_miny >abs_maxy){
			max_abs = abs_miny;
		}else{
			max_abs = abs_maxy;
		}
		if (max_abs > DELTA) {
			scaley =  static_cast<double>(HEIGHT) / max_abs;
		} else {
			scaley = 1;
		}
		std::cout << "GraphWindow::drawGraph: " << "scaley = " << scaley << std::endl;
	} else {
		scaley = 1;
	}

	double scalex;
	if (this->graphProperties & GraphProperties::SCALE_X) {
		scalex = static_cast<double>(WIDTH) / points.size();
		std::cout << "GraphWindow::drawGraph: " << "scalex = " << static_cast<double>(WIDTH) << " / " << points.size()
				<< " = " << scalex << std::endl;
	} else {
		scalex = 1;
	}

	int count_x = 0;
	it_ps = points.begin();
	if (it_ps != points.end()) {
		cr->move_to(scalex * count_x, (0.5 * HEIGHT) - (0.5 * scaley * it_ps->getY()));
	} else {
		cr->move_to(0, HEIGHT / 2);
	}
	while (it_ps != points.end()) {
		++count_x;
		double x_point_final = scalex * count_x;
		double y_point_final = (0.5 * HEIGHT) - (0.5*scaley * it_ps->getY());
		std::cout << "GraphWindow::drawGraph: " << "(" << x_point_final << ", " << y_point_final << ")" << std::endl;

		cr->line_to(x_point_final, y_point_final);
		++it_ps;
	}
	cr->stroke();
	cr->restore();
}

void GraphWindow::drawText(const Cairo::RefPtr<Cairo::Context>& cr) {
	std::cout << "GraphWindow::drawText: " << "" << std::endl;
	Glib::RefPtr<Pango::Layout> pangoLayout = Pango::Layout::create(cr);
	cr->move_to(10, 10);
	pangoLayout->set_text("text");
	pangoLayout->update_from_cairo_context(cr); //gets cairo cursor position
	pangoLayout->add_to_cairo_context(cr); //adds text to cairos stack of stuff to be drawn
	cr->stroke();
}

void GraphWindow::setSourceRGB(Cairo::RefPtr<Cairo::Context> cr, const Gdk::Color & col) {
	double r = col.get_red_p();
	double g = col.get_green_p();
	double b = col.get_blue_p();
	cr->set_source_rgb(r, g, b);
}

void GraphWindow::add(cryomesh::spacial::Point obj) {
	points.push_back(obj);
	if (graphSize >= 0) {
		int size_diff = points.size() - graphSize;
		while (size_diff > 0) {
			points.pop_front();
			--size_diff;
		}
	}
	this->invalidateWindow();
}
void GraphWindow::add(const std::initializer_list<cryomesh::spacial::Point> & list) {
	// forall in list
	{
		std::initializer_list<cryomesh::spacial::Point>::iterator it_list = list.begin();
		const std::initializer_list<cryomesh::spacial::Point>::const_iterator it_list_end = list.end();
		while (it_list != it_list_end) {
			this->add(*it_list);
			++it_list;
		}
	}
}
void GraphWindow::add(const std::list<cryomesh::spacial::Point> & list) {
	// forall in list
	{
		std::list<cryomesh::spacial::Point>::const_iterator it_list = list.begin();
		const std::list<cryomesh::spacial::Point>::const_iterator it_list_end = list.end();
		while (it_list != it_list_end) {
			this->add(*it_list);
			++it_list;
		}
	}
}
void GraphWindow::remove(cryomesh::spacial::Point obj) {
	// forall in objects
	{
		typename std::list<cryomesh::spacial::Point>::iterator it_objects = points.begin();
		typename std::list<cryomesh::spacial::Point>::const_iterator it_objects_end = points.end();

		while (it_objects != it_objects_end) {
			if (*it_objects == obj) {
				points.erase(it_objects);
				it_objects = points.begin();
				it_objects_end = points.end();
			}
			++it_objects;
		}
	}
}
void GraphWindow::remove(const std::initializer_list<cryomesh::spacial::Point> & list) {
	// forall in list
	{
		typename std::initializer_list<cryomesh::spacial::Point>::iterator it_list = list.begin();
		const typename std::initializer_list<cryomesh::spacial::Point>::const_iterator it_list_end = list.end();
		while (it_list != it_list_end) {
			this->remove(*it_list);
			++it_list;
		}
	}
}
void GraphWindow::remove(const std::list<cryomesh::spacial::Point> & list) {
	// forall in list
	{
		typename std::list<cryomesh::spacial::Point>::const_iterator it_list = list.begin();
		const typename std::list<cryomesh::spacial::Point>::const_iterator it_list_end = list.end();
		while (it_list != it_list_end) {
			this->remove(*it_list);
			++it_list;
		}
	}
}
void GraphWindow::clear() {
	points.clear();
}

} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
