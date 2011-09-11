/*
 * GraphWindow.cpp
 *
 *  Created on: 17 Aug 2011
 *      Author: niall
 */

#include "GraphWindow.h"
#include "common/Maths.h"
#include <iostream>

using namespace cryomesh::spacial;
using namespace cryomesh::common;

namespace cryo {
namespace viewer {
namespace display {

const int GraphWindow::GRAPH_PROPERTIES = SCALE_X | SCALE_Y | SHOW_MAX | SHOW_MIN;
const int GraphWindow::GRAPH_SIZE = 10;

GraphWindow::GraphWindow() :
		graphProperties(GRAPH_PROPERTIES) {
	this->clear();
	this->setSize(GRAPH_SIZE);
	this->set_size_request(GRAPH_SIZE, 50);

	graphColours.dataForeground = Gdk::Color("white");
	graphColours.dataBackground = Gdk::Color("black");
	graphColours.textForeground = Gdk::Color("grey");
	graphColours.textBackground = Gdk::Color("grey");
	graphColours.frameworkForeground = Gdk::Color("grey");
	graphColours.frameworkBackground = Gdk::Color("grey");
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

	this->calculateWindowAspect();
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
	//draw axis
	cr->save();
	cr->set_line_width(2.0);
	this->setSourceRGB(cr, graphColours.frameworkForeground);
	cr->set_font_size(12);

	cr->save();

	if (graphProperties & SHOW_MIN) {
		cr->set_dash(std::vector<double>( { 0, 1 }), 0);

		//draw min
		cr->move_to(graphWindowAspect.minXAxis, graphWindowAspect.minYAxis);
		cr->line_to(graphWindowAspect.maxXAxis, graphWindowAspect.minYAxis);
		std::cout << "GraphWindow::drawFramework: " << "minY: " << graphWindowAspect.maxXAxis << ", "
				<< graphWindowAspect.minYAxis << std::endl;
		// do axis marks
		//		cr->move_to(scalex * 190, 25 + HEIGHT / 2);
		//			cr->show_text("20");
	}
	if (graphProperties & SHOW_MAX) {
		cr->set_dash(std::vector<double>( { 0, 1 }), 0);
		//draw max
		cr->move_to(graphWindowAspect.minXAxis, graphWindowAspect.maxYAxis);
		cr->line_to(graphWindowAspect.maxXAxis, graphWindowAspect.maxYAxis);
		std::cout << "GraphWindow::drawFramework: " << "minY: " << graphWindowAspect.maxXAxis << ", "
				<< graphWindowAspect.maxYAxis << std::endl;
	}
	cr->stroke();
	cr->restore();
}

void GraphWindow::drawGraph(const Cairo::RefPtr<Cairo::Context>& cr) {
	std::cout << "GraphWindow::drawGraph: " << *this << std::endl;
	cr->save();

	cr->set_line_width(2.0);
	this->setSourceRGB(cr, Gdk::Color("white"));

	//diffx = maxx-minx;
	//diffy = maxy - miny;

	double scalex = graphWindowAspect.scale.x;
	double scaley = graphWindowAspect.scale.y;

	std::list<cryomesh::spacial::Point>::const_iterator it_ps = points.begin();

	int count = 0;

	while (it_ps != points.end()) {

		double x_point_final = scalex * (it_ps->getX() - this->minPoint.getX());
		//double y_point_final = (0.5 * HEIGHT) - (0.5 * scaley * it_ps->getY());
		double y_point_final = graphWindowAspect.maxYAxis - (scaley * it_ps->getY());
		std::cout << "GraphWindow::drawGraph: " << "(" << it_ps->getX() << ", " << it_ps->getY() << ")" << " at "
				<< "( " << x_point_final << ", " << y_point_final << ")	" << std::endl;
		if (count > 0) {
			cr->line_to(x_point_final, y_point_final);
		}
		++count;
		++it_ps;
	}
	cr->stroke();
	cr->restore();
}

void GraphWindow::drawText(const Cairo::RefPtr<Cairo::Context>& cr) {
	std::cout << "GraphWindow::drawText: " << "" << std::endl;

	double scalex = graphWindowAspect.scale.x;
	double scaley = graphWindowAspect.scale.y;

	Glib::RefPtr<Pango::Layout> pangoLayout = Pango::Layout::create(cr);
	cr->move_to(scalex * 10, scaley * 10);
	pangoLayout->set_text("text");
	pangoLayout->update_from_cairo_context(cr); //gets cairo cursor position
	pangoLayout->add_to_cairo_context(cr); //adds text to cairos stack of stuff to be drawn
	cr->stroke();
}

void GraphWindow::calculateWindowAspect() {

	// calculate axis scale
	double DELTA = 0.01;

	Gtk::Allocation allocation = get_allocation();
	const int WIDTH = allocation.get_width();
	const int HEIGHT = allocation.get_height();

	graphWindowAspect.scale.x = static_cast<double>(WIDTH) /  std::max<double>(DELTA, (maxPoint.getX() - minPoint.getX()));
	graphWindowAspect.scale.y = static_cast<double>(HEIGHT) /  std::max<double>(DELTA, (maxPoint.getY() - minPoint.getY()));
	graphWindowAspect.scale.z = 1.0;

	graphWindowAspect.scale.x = std::max<double>(DELTA, graphWindowAspect.scale.x);
	graphWindowAspect.scale.y = std::max<double>(DELTA, graphWindowAspect.scale.y);
	graphWindowAspect.scale.z = std::max<double>(DELTA, graphWindowAspect.scale.z);

	// set minmax axis
	graphWindowAspect.minXAxis = 0;
	graphWindowAspect.maxXAxis = graphWindowAspect.scale.x * maxPoint.getX();

	graphWindowAspect.minYAxis = graphWindowAspect.scale.y * minPoint.getY();
	graphWindowAspect.maxYAxis = graphWindowAspect.scale.y * maxPoint.getY();

	graphWindowAspect.minZAxis = graphWindowAspect.scale.z * minPoint.getZ();
	graphWindowAspect.maxZAxis = graphWindowAspect.scale.z * maxPoint.getZ();

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
		bool need_maxmin_check = false;
		while (size_diff > 0) {

			const Point & temp_point = points.front();
			if (this->checkPointMaxMin(temp_point) == true) {
				need_maxmin_check = true;

			}
			points.pop_front();
			--size_diff;
		}

		if (need_maxmin_check == true) {
			std::pair<Point, Point> maxmin = this->findVirtualMaxMinPoint();
			minPoint = maxmin.first;
			maxPoint = maxmin.second;
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
				std::pair<Point, Point> maxmin = this->findVirtualMaxMinPoint();
				minPoint = maxmin.first;
				maxPoint = maxmin.second;
			}
			++it_objects;
		}
	}

	//const double DELTA = 0.00000001;

	std::pair<Point, Point> maxmin = this->findVirtualMaxMinPoint();
	minPoint = maxmin.first;
	maxPoint = maxmin.second;

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
			}
			if (it_points->getX() < min_x) {
				min_x = it_points->getX();
			}

			if (it_points->getY() > max_y) {
				max_y = it_points->getY();
			}
			if (it_points->getY() < min_y) {
				min_y = it_points->getY();
			}

			if (it_points->getZ() > max_z) {
				max_z = it_points->getZ();
			}
			if (it_points->getZ() < min_z) {
				min_z = it_points->getZ();
			}

			++it_points;
		}
	}
	Point temp_min(min_x, min_y, min_z);
	Point temp_max(max_x, max_y, max_z);
	return std::pair<Point, Point>(temp_min, temp_max);
}

bool GraphWindow::checkPointMaxMin(const Point & obj) const {
	const double DELTA = 0.00001;
	if (((obj.getX() < maxPoint.getX() + DELTA) && (obj.getX() > maxPoint.getX() - DELTA))
			|| ((obj.getX() < minPoint.getX() + DELTA) && (obj.getX() > minPoint.getX() - DELTA))
			|| ((obj.getY() < maxPoint.getY() + DELTA) && (obj.getY() > maxPoint.getY() - DELTA))
			|| ((obj.getY() < minPoint.getY() + DELTA) && (obj.getY() > minPoint.getY() - DELTA))
			|| ((obj.getZ() < maxPoint.getZ() + DELTA) && (obj.getZ() > maxPoint.getZ() - DELTA))
			|| ((obj.getZ() < minPoint.getZ() + DELTA) && (obj.getZ() > minPoint.getZ() - DELTA))) {
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream & os, const GraphWindow & obj) {
	const std::list<cryomesh::spacial::Point> & all_points = obj.getPoints();
	os << "GraphWindow::operator <<: " << "" << std::endl;
	// forall in all_points
	{
		std::list<cryomesh::spacial::Point>::const_iterator it_all_points = all_points.begin();
		const std::list<cryomesh::spacial::Point>::const_iterator it_all_points_end = all_points.end();
		while (it_all_points != it_all_points_end) {
			os << "( " << it_all_points->getX() << ", " << it_all_points->getY() << ", " << it_all_points->getZ()
					<< " ) ";
			++it_all_points;
		}
	}
	return os;
}

} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
