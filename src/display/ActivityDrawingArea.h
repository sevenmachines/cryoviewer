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
	enum DisplayColourScheme {
		DEFAULT_COLOUR_SCHEME,
		STANDARD_COLOUR_SCHEME,
		INACTIVE_COLOUR_SCHEME,
		FIRED_COLOUR_SCHEME,
		PRIMARY_INPUT_COLOUR_SCHEME,
		PRIMARY_OUTPUT_COLOUR_SCHEME
	};

	struct DisplayColours {
	public:
		DisplayColours(Gdk::Color main_colour, Gdk::Color background_colour, Gdk::Color axis_colour) :
			mainColour(main_colour), backgroundColour(background_colour), axisColour(axis_colour) {
		}
		const Gdk::Color & getMainColour()const{
			return mainColour;
		}
		const Gdk::Color & getBackgroundColour() const{
			return backgroundColour;
		}
		const Gdk::Color & getAxisColour()const {
			return axisColour;
		}
		void setMainColour(Gdk::Color colour) {
			mainColour = colour;
		}
		void setBackgroundColour(Gdk::Color colour) {
			backgroundColour = colour;
		}
		void setAxisColour(Gdk::Color colour) {
			axisColour = colour;
		}
	private:
		Gdk::Color mainColour;
		Gdk::Color backgroundColour;
		Gdk::Color axisColour;
	};
	ActivityDrawingArea();
	virtual ~ActivityDrawingArea();
	virtual void update()=0;
	// enabler methods
	virtual void showDrawingArea(bool b);
	void setColourScheme(DisplayColourScheme colour_scheme);
	void setDefaultColourScheme(DisplayColourScheme colour_scheme);
	bool isActivated() const;
	void setActivated(bool b);
	void setLabelText(const std::string & str);

protected:
	//Override default signal handler:
#ifdef ENABLE_GTK2
	virtual bool on_expose_event(GdkEventExpose* event);
#else
	virtual bool on_draw(const Cairo::RefPtr< Cairo::Context >& cr);
#endif
	virtual void invalidateWindow(void);

	std::map<double, double> points;
	std::map<double, double> last_points;
	std::map<double, double>::iterator it_points;
	std::map<double, double>::iterator it_last_points;

	std::string labelText;
	static const int ACTIVITY_HEIGHT;
	bool enable_drawing_area;

	virtual void drawPoints(std::map<double, double> &, const double reference_line=0);
	virtual void drawText();
	virtual void drawAxis();
	virtual void setSourceRGB(Cairo::RefPtr<Cairo::Context> cr, const Gdk::Color & col);

	static const DisplayColours STANDARD_COLOURS;
static const DisplayColours INACTIVE_COLOURS;
	static const DisplayColours FIRED_COLOURS;
	static const DisplayColours PRIMARY_INPUT_COLOURS;
	static const DisplayColours PRIMARY_OUTPUT_COLOURS;

	DisplayColours currentColourScheme;
	DisplayColours defaultColourScheme;
	bool activated;

private:

};

}//NAMESPACE

}//NAMESPACE
}//NAMESPACE

#endif /* ActivityDrawingArea_H_ */
