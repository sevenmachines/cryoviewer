/*
 * DisplayWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef DISPLAYWINDOW_H_
#define DISPLAYWINDOW_H_

#include "common/Debuggable.h"
#include <iostream>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>

namespace cryo {

namespace viewer {

namespace display {

/**
 * Super class of all display windows
 */
class DisplayWindow: public cryomesh::common::Debuggable {
public:
	DisplayWindow();
	virtual void loadWindow(const std::string filename) ;
	virtual ~DisplayWindow() ;
	void update() ;
	virtual void activate() ;
	virtual void deactivate();
	bool isActive();
	virtual void enableDebug(bool b);

protected:
	Gtk::Window * mainWindow;
	Glib::RefPtr<Gtk::Builder> builder;

	virtual void updateData()=0;
	virtual void initialise()=0;

	void setTitle(std::string title);
private:
	bool activeState;

};

}

}

}

#endif /* DISPLAYWINDOW_H_ */
