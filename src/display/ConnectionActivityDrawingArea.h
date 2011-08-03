/*
 * ConnectionActivityDrawingArea.h
 *
 *  Created on: 3 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONNECTIONACTIVITYDRAWINGAREA_H_
#define CONNECTIONACTIVITYDRAWINGAREA_H_

#include "ActivityDrawingArea.h"
#include "components/Connection.h"
#include <boost/shared_ptr.hpp>

namespace cryo {

namespace viewer {

namespace display {

class ConnectionActivityDrawingArea: public ActivityDrawingArea {
public:
	ConnectionActivityDrawingArea(const boost::shared_ptr<cryomesh::components::Connection> & obj);
	virtual ~ConnectionActivityDrawingArea();
	void update();
protected:
	virtual void drawText();
	virtual void drawAxis();
	virtual void drawBarPoints();
	virtual void drawBar(const double x, const double y,  const int overlay_count, const Gdk::Color & colour);
	virtual void setAsPrimaryInput();
	virtual void setAsPrimaryOutput();
#ifdef ENABLE_GTK2
	virtual bool on_expose_event(GdkEventExpose* event);
#else
	virtual bool on_draw(const Cairo::RefPtr< Cairo::Context >& cr);
#endif

private:
	const boost::shared_ptr<cryomesh::components::Connection> connection;
	static const int MAX_FUTURE_TICKS;

	std::map<double, std::vector<double> > multiPoints;
	bool isPrimaryInput;
	bool isPrimaryOutput;
};

}

}

}

#endif /* CONNECTIONACTIVITYDRAWINGAREA_H_ */
