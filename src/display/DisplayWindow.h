/*
 * DisplayWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef DISPLAYWINDOW_H_
#define DISPLAYWINDOW_H_

#include <iostream>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>

namespace cryo {

namespace viewer {

namespace display {

/**
 * Super class of all display windows
 */
class DisplayWindow {
public:
	DisplayWindow() {
		mainWindow = 0;
	}
	virtual void loadWindow(const std::string filename) {
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
		std::cout << "DisplayWindow::~DisplayWindow: " << "" << std::endl;

	}
	void update() {
		std::cout << "DisplayWindow::update: " << "" << std::endl;
		if (this->isActive() == true) {
			this->updateData();
		}
	}
	virtual void activate() {
		if (mainWindow != 0) {
			mainWindow->show();
		}
		activeState = true;
	}
	virtual void deactivate() {
		if (mainWindow != 0) {
			mainWindow->hide();
		}
		activeState = false;
	}
	bool isActive() {
		return activeState;
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
