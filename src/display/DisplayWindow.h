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
	DisplayWindow() {
		mainWindow = 0;
	}
	virtual void loadWindow(const std::string filename) {
		//std::cout << "DisplayWindow::loadWindow: " << "filename: "<<filename << std::endl;
		try {
			builder = Gtk::Builder::create();
			builder->add_from_file(filename);
			builder->get_widget("mainWindow", mainWindow);
			this->initialise();
		} catch (const Glib::FileError& ex) {
			std::cerr << "DisplayWindow::loadWindow: FileError: " << ex.what() << std::endl;
		} catch (const Gtk::BuilderError& ex) {
			std::cerr << "DisplayWindow::loadWindow: BuilderError: " << ex.what() << std::endl;
		}

	}
	virtual ~DisplayWindow() {
	}
	void update() {
		if (this->isActive() == true) {
			this->updateData();
		}
	}
	virtual void activate() {
		activeState = true;
		if (mainWindow != 0) {
			//std::cout << "DisplayWindow::activate: " << "show" << std::endl;
			this->update();
			mainWindow->show();
		}
	}
	virtual void deactivate() {
		activeState = false;
		if (mainWindow != 0) {
			//std::cout << "DisplayWindow::deactivate: " << "hide" << std::endl;
			mainWindow->hide();
		}
	}
	bool isActive() {
		return activeState;
	}
	virtual void enableDebug(bool b){
		this->setDebug(b);
	}

protected:
	Gtk::Window * mainWindow;
	Glib::RefPtr<Gtk::Builder> builder;

	virtual void updateData()=0;
	virtual void initialise()=0;

	void setTitle(std::string title) {
		mainWindow->set_title(title);
	}
private:
	bool activeState;

};

}

}

}

#endif /* DISPLAYWINDOW_H_ */
