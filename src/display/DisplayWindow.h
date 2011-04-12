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
		std::cout << "DisplayWindow::DisplayWindow: " << "" << std::endl;
		mainWindow = 0;
	}
	virtual void loadWindow(const std::string filename) {
		std::cout << "DisplayWindow::loadWindow: " << filename << std::endl;
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
		std::cout<<"DisplayWindow::activate: "<<""<<std::endl;
		if (mainWindow != 0) {
			std::cout << "DisplayWindow::activate: " << "show" << std::endl;
			mainWindow->show();
		}
		activeState = true;
	}
	virtual void deactivate() {
		std::cout<<"DisplayWindow::deactivate: "<<""<<std::endl;
		if (mainWindow != 0) {
			std::cout << "DisplayWindow::deactivate: " << "hide" << std::endl;
			mainWindow->hide();
		}
		activeState = false;
	}
	bool isActive() {
		return activeState;
	}

protected:
	virtual void updateData()=0;
	virtual void initialise()=0;

private:
	bool activeState;
	Gtk::Window * mainWindow;
	Glib::RefPtr<Gtk::Builder> builder;
};

}

}

}

#endif /* DISPLAYWINDOW_H_ */
