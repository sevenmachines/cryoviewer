/*
 * GraphWindow.h
 *
 *  Created on: 17 Aug 2011
 *      Author: niall
 */

#ifndef GRAPHWINDOW_H_
#define GRAPHWINDOW_H_

#include "spacial/Point.h"
#include <gtkmm/drawingarea.h>

namespace cryo {
namespace viewer {
namespace display {

class GraphWindow: public Gtk::DrawingArea {
public:

	enum GraphAxisDisplayProperty {
		SCALE_Y = 1, SCALE_X = 2, SHOW_MIN = 4, SHOW_MAX = 8
	};

	enum GraphTextDisplayProperty {
		SHOW_MAX_TEXT = 1, SHOW_MIN_TEXT = 2, SHOW_VARIATION_TEXT = 4, SHOW_AVERAGE_TEXT = 8
	};

	GraphWindow();
	virtual ~GraphWindow();

	const std::list<cryomesh::spacial::Point> & getPoints() const;

	void setSize(const int sz);

	/**
	 * Access underlying collection methods
	 */
	virtual void add(cryomesh::spacial::Point obj);
	virtual void add(const std::initializer_list<cryomesh::spacial::Point> & list);
	virtual void add(const std::list<cryomesh::spacial::Point> & list);
	virtual void remove(cryomesh::spacial::Point obj);
	virtual void remove(const std::initializer_list<cryomesh::spacial::Point> & list);
	virtual void remove(const std::list<cryomesh::spacial::Point> & list);
	virtual void clear();

	void enableGraphAxisDisplayProperty(const GraphAxisDisplayProperty property) ;
	void disableGraphAxisDisplayProperty(const GraphAxisDisplayProperty property) ;
	void enableGraphTextDisplayProperty(const GraphTextDisplayProperty property) ;
	void disableGraphTextDisplayProperty(const GraphTextDisplayProperty property) ;

	/**
	 * To stream operator
	 *
	 *	@param std::ostream & os
	 *		The output stream
	 *	@param const GraphWindow & obj
	 *		The object to stream
	 *
	 *	@return std::ostream &
	 *		The output stream
	 */
	friend std::ostream& operator<<(std::ostream & os, const GraphWindow & obj);

protected:
#ifdef ENABLE_GTK2
	virtual bool on_expose_event(GdkEventExpose* event);
#else
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
#endif
	virtual void invalidateWindow(void);

	/**
	 * Draw the underlying frameworks such as axis to overlay our graph on
	 */
	virtual void drawFramework(const Cairo::RefPtr<Cairo::Context>& cr);

	/**
	 * Draw out the actual graph data
	 */
	virtual void drawGraph(const Cairo::RefPtr<Cairo::Context>& cr);

	/**
	 * Draw text such as labelling
	 */
	virtual void drawText(const Cairo::RefPtr<Cairo::Context>& cr);

	/**
	 * Calculate window aspect and scaling
	 */
	virtual void calculateWindowAspect();

	void setSourceRGB(Cairo::RefPtr<Cairo::Context> cr, const Gdk::Color & col);

	const static int DEFAULT_GRAPH_AXIS_DISPLAY_PROPERTIES;
	const static int DEFAULT_GRAPH_SIZE;
	const static int DEFAULT_GRAPH_TEXT_DISPLAY_PROPERTIES;

private:
	int graphAxisDisplayProperties;
	int graphTextDisplayProperties;
	int graphSize;
	std::list<cryomesh::spacial::Point> points;

	/**
	 * Structure to hold variables relating to the window aspect and scaling
	 */
	struct GraphWindowAspect {
		GraphWindowAspect() :
				xAxis(0), yAxis(0), zAxis(0) {
		}
		double xAxis;
		double yAxis;
		double zAxis;

		double minXAxis;
		double minYAxis;
		double minZAxis;

		double maxXAxis;
		double maxYAxis;
		double maxZAxis;

		struct Scale {
			Scale() :
					x(1.0), y(1.0), z(1.0) {
			}
			double x;
			double y;
			double z;
		} scale;
	} graphWindowAspect;

	/**
	 * Point representing the max x, y and z values within the data set
	 */
	cryomesh::spacial::Point maxPoint;
	/**
	 * Point representing the min x, y and z values within the data set
	 */
	cryomesh::spacial::Point minPoint;

	/**
		 * Point representing the average x, y and z values within the data set
		 */
		cryomesh::spacial::Point averagePoint;

	/**
	 * Find max an min and average values for x y and z in data set
	 */
	void  findVirtualPoints(cryomesh::spacial::Point & min_point, cryomesh::spacial::Point & max_point, cryomesh::spacial::Point & av_point) const;

	bool checkPointMaxMin(const cryomesh::spacial::Point & obj) const;

	struct GraphColours {
		Gdk::Color frameworkForeground;
		Gdk::Color frameworkBackground;
		Gdk::Color textForeground;
		Gdk::Color textBackground;
		Gdk::Color dataForeground;
		Gdk::Color dataBackground;
	} graphColours;

};

} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
#endif /* GRAPHWINDOW_H_ */
