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

#include <gtkmm/gl/init.h>
#include <gtkmm/gl/version.h>

#include <gtkmm/builder.h>

namespace cryo {
namespace viewer {

const std::string cryo::viewer::MainWindow::DEFAULT_CONFIG_FILE =
		"/home/niall/Projects/Eclipse/cryomesh-cute/TestData/basic-2c.config";
const std::string cryo::viewer::MainWindow::DEFAULT_UI_FILE = "Data/mainwindow.glade";

MainWindow::MainWindow(std::string filename, int argc, char **argv) {
	Gtk::Main kit(argc, argv);
	//init gl
	Gtk::GL::init(argc, argv);

	if (filename == "" || this->checkFileExists(filename) == false) {
		std::cout << "MainWindow: " << "File " << "'" << filename << "'" << " does not exist..." << std::endl;
		filename = cryo::viewer::MainWindow::getFileChoice();
	}
	bool filetype_extension_found = (filename.find(".config") != std::string::npos);
	if (filetype_extension_found == false) {
		std::cout << "MainWindow::MainWindow: "
				<< "ERROR: Config file does not have a '.config' extension, ignoring..." << std::endl;
	}
	// if still no choice then dont bother starting
	if (filename != "" && this->checkFileExists(filename) == true && filetype_extension_found) {
		// create manager
		manager = boost::shared_ptr<cryomesh::manager::CryoManager>(new cryomesh::manager::CryoManager);
		manager->create(filename);
		try {
			mainWindowBuilder = Gtk::Builder::create();
			mainWindowBuilder->add_from_file(MainWindow::DEFAULT_UI_FILE);
			this->initialise();
			Gtk::Main::run(*mainWindow);
		} catch (const Glib::FileError& ex) {
			std::cerr << "main: FileError: " << ex.what() << std::endl;
		} catch (const Gtk::BuilderError& ex) {
			std::cerr << "main: BuilderError: " << ex.what() << std::endl;
		}
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
	mainWindowBuilder->get_widget("mainWindowRunSpinButton", mainWindowRunSpinButton);
	mainWindowBuilder->get_widget("mainWindowProgressBar", mainWindowProgressBar);
	mainWindowBuilder->get_widget("mainWindowDebugEnabledCheckButton", mainWindowDebugEnabledCheckButton);

	mainWindowBuilder->get_widget("structureButtonImage", structureButtonImage);

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

	mainWindowDebugEnabledCheckButton->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::onMainWindowDebugEnabledCheckButtonClicked));

	mainWindow->signal_delete_event().connect(sigc::mem_fun(*this, &MainWindow::onDeleteEvent));

	Gtk::Settings::get_default()->property_gtk_button_images() = true;
}

void MainWindow::onMainWindowToggleButtonNodesClicked() {
	std::cout << "MainWindow::onMainWindowToggleButtonNodesClicked: " << mainWindowToggleButtonNodes->get_active()
			<< std::endl;

}

void MainWindow::onMainWindowToggleButtonStructureClicked() {
	this->onMainWindowToggleButtonClicked<display::StructureWindow> (mainWindowToggleButtonStructure, structureWindow);
}
void MainWindow::onMainWindowToggleButtonStatisticsClicked() {
	this->onMainWindowToggleButtonClicked<display::StatisticsWindow> (mainWindowToggleButtonStatistics,
			statisticsWindow);

}

void MainWindow::onMainWindowToggleButtonVisualiseClicked() {
	std::cout << "MainWindow::mainWindowToggleButtonVisualise: " << mainWindowToggleButtonVisualise->get_active()
			<< std::endl;
}
void MainWindow::onMainWindowToggleButtonRunClicked() {
	const int cycle_count = mainWindowRunSpinButton->get_value_as_int();
	if (mainWindowToggleButtonRun->get_active()) {
		mainWindowProgressBar->set_fraction(0);
		for (int i = 0; i < cycle_count && mainWindowToggleButtonRun->get_active(); i++) {
			manager->runCycle();
			double fraction_complete = static_cast<double> (i + 1) / static_cast<double> (cycle_count);
			mainWindowProgressBar->set_fraction(fraction_complete);
			// update all windows
			if (statisticsWindow != 0) {
				statisticsWindow->update();
			}
			// update all windows
			if (structureWindow != 0) {
				structureWindow->update();
			}
			// update all windows
			if (dataWindow != 0) {
				dataWindow->update();
			}

			this->processPendingEvents();
		}
		mainWindowToggleButtonRun->set_active(false);
	}

}

template<class T>
void MainWindow::onMainWindowToggleButtonClicked(Gtk::ToggleButton * togglebutton,
		boost::shared_ptr<T> & display_window) {

	if (togglebutton->get_active() == true) {
		if (manager != 0) {
			if (display_window == 0) {
				display_window = boost::shared_ptr<T>(new T(manager->getBundle()));
			}
			display_window->activate();

		}
	} else if (display_window != 0) {
		display_window->deactivate();
	}
}

void MainWindow::onMainWindowDebugEnabledCheckButtonClicked() {
	bool enable_debug = mainWindowDebugEnabledCheckButton->get_active() == true;
	std::cout<<"MainWindow::onMainWindowDebugEnabledCheckButtonClicked: "<<enable_debug<<std::endl;

	this->manager->getMutableBundle()->enableDebug(enable_debug);
}

void MainWindow::processPendingEvents() {
	// allow gui to process all events
	mainWindow->queue_draw();
	while (Gtk::Main::events_pending()) {
		Gtk::Main::iteration();
	}
}

bool MainWindow::onDeleteEvent(GdkEventAny *) {
	// check we really want to quit
	Gtk::MessageDialog dialog("Are you sure you want to quit");
	dialog.set_transient_for(*mainWindow);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	//	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	int result = dialog.run();

	//Handle the response:
	if (result == Gtk::RESPONSE_OK) {
		std::cout << "MainWindow::onDeleteEvent: " << "TODO: Check save" << std::endl;
		Gtk::Main::quit();
		return true;
	} else if (result == Gtk::RESPONSE_CANCEL) {
		return true;
	}

	return false;
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

