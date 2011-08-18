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

	enum GraphProperties {
		SCALE_Y = 1, SCALE_X = 2
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

	void setSourceRGB(Cairo::RefPtr<Cairo::Context> cr, const Gdk::Color & col);

	static const int GRAPH_PROPERTIES;
	static const int GRAPH_SIZE;

private:
	int graphProperties;
	int graphSize;
	std::list<cryomesh::spacial::Point> points;
};

} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
#endif /* GRAPHWINDOW_H_ */
