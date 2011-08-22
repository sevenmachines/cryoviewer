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

const int GraphWindow::GRAPH_PROPERTIES = SCALE_X | SCALE_Y | SHOW_MAX | SHOW_MIN;
const int GraphWindow::GRAPH_SIZE = 10;

GraphWindow::GraphWindow() :
		graphProperties(GRAPH_PROPERTIES), xAxis(0), yAxis(0), zAxis(0) {
	this->clear();
	this->setSize(GRAPH_SIZE);
	this->set_size_request(GRAPH_SIZE, 50);

	graphColours.dataForeground=Gdk::Color("white");
	graphColours.dataBackground=Gdk::Color("black");
	graphColours.textForeground=Gdk::Color("grey");
	graphColours.textBackground=Gdk::Color("grey");
	graphColours.frameworkForeground=Gdk::Color("grey");
	graphColours.frameworkBackground=Gdk::Color("grey");
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
	this->setSourceRGB(cr, graphColours.dataBackground);
	cr->paint();

	std::pair<double, double> scale;
	bool do_scalex = (graphProperties & SCALE_X);
	bool do_scale_y = (graphProperties & SCALE_Y);
	if (do_scalex || do_scale_y) {
		scale = this->getAxisScale();
		if (do_scalex == false) {
			scale.first = 1.0;
		}
		if (do_scale_y == false) {
			scale.second = 1.0;
		}
	}
	drawGraph(cr, scale);
	drawFramework(cr, scale);
	drawText(cr, scale);
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

void GraphWindow::drawFramework(const Cairo::RefPtr<Cairo::Context>& cr, const std::pair<double, double> scale) {
	Gtk::Allocation allocation = get_allocation();
	const int WIDTH = allocation.get_width();
	const int HEIGHT = allocation.get_height();

	double scalex = scale.first;
	double scaley = scale.second;

	//draw axis
	cr->save();
	cr->set_line_width(2.0);
	this->setSourceRGB(cr,  graphColours.frameworkForeground);
	cr->set_font_size(12);

	cr->move_to(0, HEIGHT / 2);
	cr->line_to(WIDTH, HEIGHT / 2);

	cr->move_to(scalex * 190, 25 + HEIGHT / 2);
	cr->show_text("20");
	cr->move_to(scalex * 390, 25 + HEIGHT / 2);
	cr->show_text("40");
	cr->move_to(scalex * 590, 25 + HEIGHT / 2);
	cr->show_text("60");
	cr->move_to(scalex * 790, 25 + HEIGHT / 2);
	cr->show_text("80");
	cr->stroke();

	// Draw max mins
	cr->set_dash(std::vector<double>({0,1}), 0);
	if (graphProperties & SHOW_MIN) {
		cr->move_to(0,  (0.5 * HEIGHT) -(0.5 *scaley * minPoint.getY()));
		cr->line_to(WIDTH,  (0.5 * HEIGHT) -(0.5 *scaley * minPoint.getY()));
		std::cout<<"GraphWindow::drawFramework: "<<"MIN_AXIS: "<<"("<<WIDTH <<","<< (scaley * minPoint.getY())<<")"<<std::endl;
	}
	if (graphProperties & SHOW_MAX) {
		cr->move_to(0, (0.5 * HEIGHT) -(0.5 * scaley * maxPoint.getY()));
		cr->line_to(WIDTH,  (0.5 * HEIGHT) -(0.5 *scaley * maxPoint.getY()));
		std::cout<<"GraphWindow::drawFramework: "<<"MAX_AXIS: "<<"("<<WIDTH <<","<< (scaley * maxPoint.getY())<<")"<<std::endl;
	}
	cr->stroke();
	cr->restore();
}

void GraphWindow::drawGraph(const Cairo::RefPtr<Cairo::Context>& cr, const std::pair<double, double> scale) {
	std::cout << "GraphWindow::drawGraph: " << "" << std::endl;
	cr->save();
	Gtk::Allocation allocation = get_allocation();
	//const int WIDTH = allocation.get_width();
	const int HEIGHT = allocation.get_height();

	cr->set_line_width(2.0);
	this->setSourceRGB(cr, Gdk::Color("white"));

	//diffx = maxx-minx;
	//diffy = maxy - miny;

	double scalex = scale.first;
	double scaley = scale.second;

	int count_x = 0;
	std::list<cryomesh::spacial::Point>::const_iterator it_ps = points.begin();
	if (it_ps != points.end()) {
		cr->move_to(scalex * count_x, (0.5 * HEIGHT) - (0.5 * scaley * it_ps->getY()));
	} else {
		cr->move_to(0, HEIGHT / 2);
	}
	while (it_ps != points.end()) {
		++count_x;
		double x_point_final = scalex * count_x;
		double y_point_final = (0.5 * HEIGHT) - (0.5 * scaley * it_ps->getY());
		std::cout << "GraphWindow::drawGraph: " << "(" << x_point_final << ", " << y_point_final << ")" << std::endl;

		cr->line_to(x_point_final, y_point_final);
		++it_ps;
	}
	cr->stroke();
	cr->restore();
}

void GraphWindow::drawText(const Cairo::RefPtr<Cairo::Context>& cr, const std::pair<double, double> scale) {
	std::cout << "GraphWindow::drawText: " << "" << std::endl;

	double scalex = scale.first;
	double scaley = scale.second;
	Glib::RefPtr<Pango::Layout> pangoLayout = Pango::Layout::create(cr);
	cr->move_to(scalex * 10, scaley * 10);
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

	if (obj.getX() > maxPoint.getX()) {
		maxPoint = Point(obj.getX(), maxPoint.getY(), maxPoint.getZ());
	} else if (obj.getX() < minPoint.getX()) {
		minPoint = Point(obj.getX(), maxPoint.getY(), maxPoint.getZ());
	}

	if (obj.getY() > maxPoint.getY()) {
		maxPoint = Point(maxPoint.getX(), obj.getY(), maxPoint.getZ());
	} else if (obj.getY() < minPoint.getY()) {
		minPoint = Point(maxPoint.getX(), obj.getY(), maxPoint.getZ());
	}

	if (obj.getZ() > maxPoint.getZ()) {
		maxPoint = Point(maxPoint.getX(), maxPoint.getY(), obj.getZ());
	} else if (obj.getZ() < minPoint.getZ()) {
		minPoint = Point(maxPoint.getX(), maxPoint.getY(), obj.getZ());
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

	const double DELTA = 0.00000001;

	std::pair<Point, Point> maxmin;

	if (((obj.getX() < maxPoint.getX() + DELTA) && (obj.getX() > maxPoint.getX() - DELTA))
			|| ((obj.getX() < minPoint.getX() + DELTA) && (obj.getX() > minPoint.getX() - DELTA))
			|| ((obj.getY() < maxPoint.getY() + DELTA) && (obj.getY() > maxPoint.getY() - DELTA))
			|| ((obj.getY() < minPoint.getY() + DELTA) && (obj.getY() > minPoint.getY() - DELTA))
			|| ((obj.getZ() < maxPoint.getZ() + DELTA) && (obj.getZ() > maxPoint.getZ() - DELTA))
			|| ((obj.getZ() < minPoint.getZ() + DELTA) && (obj.getZ() > minPoint.getZ() - DELTA))) {
		maxmin = this->findVirtualMaxMinPoint();
		minPoint = maxmin.first;
		maxPoint = maxmin.second;
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

std::pair<Point, Point> GraphWindow::findVirtualMaxMinPoint() const {
	const double MAX_CUTOFF = 10000000;
	double max_x = -MAX_CUTOFF;
	double max_y = -MAX_CUTOFF;
	double max_z = -MAX_CUTOFF;
	double min_x = MAX_CUTOFF;
	double min_y = MAX_CUTOFF;
	double min_z = MAX_CUTOFF;
	// forall in point
	{
		std::list<cryomesh::spacial::Point>::const_iterator it_points = points.begin();
		const std::list<cryomesh::spacial::Point>::const_iterator it_points_end = points.end();
		while (it_points != it_points_end) {

			if (it_points->getX() > max_x) {
				max_x = it_points->getX();
			} else if (it_points->getX() < min_x) {
				min_x = it_points->getX();
			}

			if (it_points->getY() > max_y) {
				max_y = it_points->getY();
			} else if (it_points->getY() < min_y) {
				min_y = it_points->getY();
			}

			if (it_points->getZ() > max_z) {
				max_z = it_points->getZ();
			} else if (it_points->getZ() < min_z) {
				min_z = it_points->getZ();
			}

			++it_points;
		}
	}
	return std::pair<Point, Point>(Point(min_x, min_y, min_z), Point(max_x, max_y, max_z));
}

std::pair<double, double> GraphWindow::getAxisScale() const {
	double DELTA = 0.01;
	double scalex;
	double scaley;

	Gtk::Allocation allocation = get_allocation();
	const int WIDTH = allocation.get_width();
	const int HEIGHT = allocation.get_height();

	double abs_miny = Maths::absolute<double>(minPoint.getY());
	double abs_maxy = Maths::absolute<double>(maxPoint.getY());
	double max_abs;
	if (abs_miny > abs_maxy) {
		max_abs = abs_miny;
	} else {
		max_abs = abs_maxy;
	}
	if (max_abs > DELTA) {
		scaley = static_cast<double>(HEIGHT) / max_abs;
	} else {
		scaley = 1;
	}

	scalex = static_cast<double>(WIDTH) / points.size();

	return std::pair<double, double>(scalex, scaley);
}
} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
