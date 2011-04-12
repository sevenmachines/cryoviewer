/*
 * MainWindow.cpp
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "MainWindow.h"
#include "display/StatisticsWindow.h"
#include "display/StructureWindow.h"
#include "display/DataWindow.h"
#include "display/ActiveWindow.h"

#include "manager/Creator.h"

#include <iostream>

#include <gtkmm/builder.h>

namespace cryo {
namespace viewer {

const std::string cryo::viewer::MainWindow::DATA_DIR = "Data";

const std::string cryo::viewer::MainWindow::DEFAULT_CONFIG_FILE =
		"/home/niall/Projects/Eclipse/cryomesh-cute/Data/basic-2c.config";

MainWindow::MainWindow(std::string filename, int argc, char **argv) {
	Gtk::Main kit(argc, argv);
	while (filename == "" || this->checkFileExists(filename) == false) {
		std::cout << "MainWindow: " << "File " << "'" << filename << "'" << " does not exist..." << std::endl;
		filename = cryo::viewer::MainWindow::getFileChoice();
	}
	// create manager
	manager = boost::shared_ptr<cryomesh::manager::CryoManager>(new cryomesh::manager::CryoManager);
	manager->create(filename);
	try {
		std::stringstream filepath;
		filepath << cryo::viewer::MainWindow::DATA_DIR << "/" << "mainwindow.ui";
		mainWindowBuilder = Gtk::Builder::create();
		mainWindowBuilder->add_from_file(filepath.str());
		this->initialise();
		Gtk::Main::run(*mainWindow);
	} catch (const Glib::FileError& ex) {
		std::cerr << "main: FileError: " << ex.what() << std::endl;
	} catch (const Gtk::BuilderError& ex) {
		std::cerr << "main: BuilderError: " << ex.what() << std::endl;
	}
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

void MainWindow::initialise() {
	mainWindowBuilder->get_widget("mainWindow", mainWindow);

	mainWindowBuilder->get_widget("mainWindowToggleButtonNodes", mainWindowToggleButtonNodes);
	mainWindowBuilder->get_widget("mainWindowToggleButtonStructure", mainWindowToggleButtonStructure);
	mainWindowBuilder->get_widget("mainWindowToggleButtonStatistics", mainWindowToggleButtonStatistics);
	mainWindowBuilder->get_widget("mainWindowToggleButtonVisualise", mainWindowToggleButtonVisualise);
	mainWindowBuilder->get_widget("mainWindowToggleButtonRun", mainWindowToggleButtonRun);

	// connect up togglebuttons
	mainWindowToggleButtonNodes->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::onMainWindowToggleButtonNodesClicked));
	mainWindowToggleButtonStructure->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::onMainWindowToggleButtonStructureClicked));
	mainWindowToggleButtonStatistics->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::onMainWindowToggleButtonStatisticsClicked));
	mainWindowToggleButtonVisualise->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::onMainWindowToggleButtonVisualiseClicked));
	mainWindowToggleButtonRun->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::onMainWindowToggleButtonRunClicked));

}

void MainWindow::onMainWindowToggleButtonNodesClicked() {
	std::cout << "MainWindow::onMainWindowToggleButtonNodesClicked: " << mainWindowToggleButtonNodes->get_active()
			<< std::endl;

}

void MainWindow::onMainWindowToggleButtonStructureClicked() {
	std::cout << "MainWindow::mainWindowToggleButtonStructure: " << mainWindowToggleButtonStructure->get_active()
			<< std::endl;
	this->onMainWindowToggleButtonClicked<display::StructureWindow> (mainWindowToggleButtonStructure, structureWindow);
}
void MainWindow::onMainWindowToggleButtonStatisticsClicked() {
	std::cout << "MainWindow::mainWindowToggleButtonStatistics: " << mainWindowToggleButtonStatistics->get_active()
			<< std::endl;
}

void MainWindow::onMainWindowToggleButtonVisualiseClicked() {
	std::cout << "MainWindow::mainWindowToggleButtonVisualise: " << mainWindowToggleButtonVisualise->get_active()
			<< std::endl;
}
void MainWindow::onMainWindowToggleButtonRunClicked() {
	std::cout << "MainWindow::mainWindowToggleButtonRun: " << mainWindowToggleButtonRun->get_active() << std::endl;
}

template<class T>
void MainWindow::onMainWindowToggleButtonClicked(Gtk::ToggleButton * togglebutton, boost::shared_ptr<T> & display_window) {
	std::cout << "MainWindow::mainWindowToggleButtonClicked: " << togglebutton->get_active() << std::endl;

	if (togglebutton->get_active() == true) {
		if (manager != 0) {
			if (display_window == 0) {
				display_window = boost::shared_ptr<T>(new T(manager->getBundle()));
			}
			display_window->activate();
			display_window->update();

		}
	} else if (display_window != 0) {
		display_window->deactivate();
	}
}

std::string MainWindow::getFileChoice() const {

	std::string filename = "";
	Gtk::FileChooserDialog dialog("Please choose a config file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	//dialog.set_transient_for(*this);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);

	int result = dialog.run();

	//Handle the response:
	switch (result) {
	case (Gtk::RESPONSE_OK): {
		filename = dialog.get_filename();
		break;
	}
	case (Gtk::RESPONSE_CANCEL): {
		break;
	}
	default: {
		std::cout << "Main::getFileChoice: " << "WARNING: Unexpected button clicked." << std::endl;
		assert(false);
		break;
	}
	}
	return filename;

}

bool MainWindow::checkFileExists(const std::string filename) const {
	bool success = true;
	std::ifstream ifs(filename.c_str());
	if (ifs.good() == false) {
		success = false;
	}
	return success;
}

}//NAMESPACE
}//NAMESPACE

int main(int argc, char **argv) {
	std::string firstarg = "";

	//get first arg as config file to open
	if (argc > 1) {
		firstarg = std::string(argv[1]);
	}

	std::string filename = "";
	// if arg is defaults then just use defaults for everything
	if (firstarg == "defaults") {
		filename = cryo::viewer::MainWindow::DEFAULT_CONFIG_FILE;
	} else if (firstarg == "help") {
		std::cout << "cryoviewer" << "\t\t - start cryoviewer with config file chooser" << std::endl;
		std::cout << "cryoviewer <filename>" << "\t - start cryoviewer with config file" << std::endl;
		std::cout << "cryoviewer defaults" << "\t - start cryoviewer with defaults" << std::endl;
		std::cout << "cryoviewer help" << "\t\t - show help" << std::endl;
		return 0;
	} else {
		filename = firstarg;
	}

	cryo::viewer::MainWindow window(filename, argc, argv);
	return 0;
}

