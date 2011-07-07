/*
 * SimpleGLDrawingArea.h
 *
 *  Created on: 22 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef SIMPLEGLDRAWINGAREA_H_
#define SIMPLEGLDRAWINGAREA_H_
#include <boost/shared_ptr.hpp>
#include "SimpleGLShape.h"

namespace cryo {

namespace viewer {

namespace common {

class SimpleGLDrawingArea: public Gtk::GL::DrawingArea {
public:
	SimpleGLDrawingArea();
	virtual ~SimpleGLDrawingArea();

	boost::shared_ptr<SimpleGLShape> add(boost::shared_ptr<SimpleGLShape> shape);

	boost::shared_ptr<SimpleGLShape> remove(boost::shared_ptr<SimpleGLShape> shape);
	virtual void update();

	void clear();

protected:

	virtual void on_realize() {
		this->onRealize();
	}
	virtual bool on_configure_event(GdkEventConfigure* event) {
		return this->onConfigureEvent(event);
	}
	virtual bool on_expose_event(GdkEventExpose* event) {
		return this->onExposeEvent(event);
	}

	virtual void initialise();
	virtual void onRealize();
	virtual bool onConfigureEvent(GdkEventConfigure* event);
	virtual bool onExposeEvent(GdkEventExpose* event);
	void invalidate();
	bool on_button_press_event(GdkEventButton* event) ;

	bool on_button_release_event(GdkEventButton* event) ;

	bool on_motion_notify_event(GdkEventMotion* event);

		virtual void drawShapes();

private:
	std::map<boost::uuids::uuid, boost::shared_ptr<SimpleGLShape> > shapes;
};

}

}

}

#endif /* SIMPLEGLDRAWINGAREA_H_ */
