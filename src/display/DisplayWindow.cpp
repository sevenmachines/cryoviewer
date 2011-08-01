/*
 * DisplayWindow.cpp
 *
 *  Created on: 1 Aug 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */
#include "DisplayWindow.h"
namespace cryo {

namespace viewer {

namespace display {

DisplayWindow::DisplayWindow() {
	mainWindow = 0;
}
DisplayWindow::~DisplayWindow() {
}
void DisplayWindow::update() {
	if (this->isActive() == true) {
		this->updateData();
	}
}
void DisplayWindow::activate() {
	activeState = true;
	if (mainWindow != 0) {
		//std::cout << "DisplayWindow::activate: " << "show" << std::endl;
		this->update();
		mainWindow->show();
	}
}
void DisplayWindow::deactivate() {
	activeState = false;
	if (mainWindow != 0) {
		//std::cout << "DisplayWindow::deactivate: " << "hide" << std::endl;
		mainWindow->hide();
	}
}
bool DisplayWindow::isActive() {
	return activeState;
}
void DisplayWindow::enableDebug(bool b) {
	this->setDebug(b);
}
void DisplayWindow::loadWindow(const std::string filename) {
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
void DisplayWindow::setTitle(std::string title) {
	mainWindow->set_title(title);
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
