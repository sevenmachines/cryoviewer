/*
 * ActivityDrawingArea.h
 *
 *  Created on: 21-May-2009
 *      Author: niall
 */

#ifndef ActivityDrawingArea_H_
#define ActivityDrawingArea_H_

#include "components/Node.h"
#include <gtkmm/drawingarea.h>
#include <map>
#include <sstream>

/*
 *
 */
namespace cryo {

namespace viewer {

namespace display {

class ActivityDrawingArea: public Gtk::DrawingArea {
public:
	ActivityDrawingArea();
	virtual ~ActivityDrawingArea();
	virtual void update()=0;
	// enabler methods
	virtual void showDrawingArea(bool b);
	void setMainColour(Gdk::Color col);
	void setFiredColour(Gdk::Color col);
	void setAxisColour(Gdk::Color col);
	void setBackgroundColour(Gdk::Color col);
	bool isActivated()const;
void setActivated(bool b);
	void setLabelText(const std::string & str);
protected:
	//Override default signal handler:
	virtual bool on_expose_event(GdkEventExpose* event);
	virtual void invalidateWindow(void) ;

protected:
	Gdk::Color colour;
	Gdk::Color fired_colour;
	Gdk::Color main_colour;
	Gdk::Color axis_colour;
	Gdk::Color backgroundColour;
	Gdk::Color activeBackgroundColour;
	Gdk::Color inactiveBackgroundColour;

	std::map<double, double> points;
	std::map<double, double> last_points;
	std::map<double, double>::iterator it_points;
	std::map<double, double>::iterator it_last_points;

	std::string labelText;
	static const int ACTIVITY_HEIGHT;
	bool enable_drawing_area;

	virtual void drawPoints(std::map<double, double> &);
	virtual void drawText();
	virtual void drawAxis();
	virtual void setSourceRGB(Cairo::RefPtr<Cairo::Context> cr, Gdk::Color & col);
private:
	bool activated;
};

}//NAMESPACE

}//NAMESPACE
}//NAMESPACE

#endif /* ActivityDrawingArea_H_ */
