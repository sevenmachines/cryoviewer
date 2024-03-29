/*
 * MainWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "display/StatisticsWindow.h"
#include "display/StructureWindow.h"
#include "display/DataWindow.h"
#include "display/ActiveWindow.h"
#include "display/OverviewWindow.h"
#include "manager/CryoManager.h"

#include <gtkmm.h>
#include <string>
#include <boost/shared_ptr.hpp>

namespace cryo {
namespace viewer {

class MainWindow {
public:
	MainWindow(std::string filename, int argc, char **argv);
	virtual ~MainWindow();

	virtual void initialise();
	std::string getFileChoice() const;
	bool checkFileExists(const std::string filename) const;

	/**
	 * Signal handler for when nodes toggle button is clicked
	 */
	void onMainWindowToggleButtonNodesClicked();

	/**
	 * Signal handler for when structure toggle button is clicked
	 */
	void onMainWindowToggleButtonStructureClicked();

	/**
	 * Signal handler for when statistics toggle button is clicked
	 */
	void onMainWindowToggleButtonStatisticsClicked();

	/**
	 * Signal handler for when visualise toggle button is clicked
	 */
	void onMainWindowToggleButtonVisualiseClicked();

	/**
	 * Signal handler for when enable debug check button is clicked
	 */
	void onMainWindowDebugEnabledCheckButtonClicked();

	/**
	 * Signal handler for when run toggle button is clicked
	 */
	void onMainWindowToggleButtonRunClicked();

	/**
	 * Signal handler for when overview toggle button is clicked
	 */
	void onMainWindowToggleButtonOverviewClicked();

	bool onDeleteEvent(GdkEventAny *);

	void processPendingEvents();

	/**
	 * The default config file to use if needed
	 *
	 * @var std::string
	 */
	static const std::string DEFAULT_CONFIG_FILE;
	static const std::string DEFAULT_UI_FILE;

protected:
	Glib::RefPtr<Gtk::Builder> mainWindowBuilder;
	Gtk::Window* mainWindow;
	Gtk::ToggleButton * mainWindowToggleButtonNodes;
	Gtk::ToggleButton * mainWindowToggleButtonStructure;
	Gtk::ToggleButton * mainWindowToggleButtonStatistics;
	Gtk::ToggleButton * mainWindowToggleButtonVisualise;
	Gtk::ToggleButton * mainWindowToggleButtonRun;
	Gtk::ToggleButton * mainWindowToggleButtonOverview;

	Gtk::SpinButton * mainWindowRunSpinButton;
	Gtk::ProgressBar * mainWindowProgressBar;
	Gtk::Image * structureButtonImage;
	Gtk::CheckButton * mainWindowDebugEnabledCheckButton;
	/**
	 * Template class to handle the toggle button signals for the range of similar DisplayWindow  classes
	 *
	 * @param Gtk::ToggleButton *
	 * 	The ToggleButton that was triggered
	 * @param boost::shared_ptr<T>
	 * 	The template pointer to the actual display window to work with
	 */
	template<class T>
	void onMainWindowToggleButtonClicked(Gtk::ToggleButton * togglebutton, boost::shared_ptr<T> & display_window);

private:
	boost::shared_ptr<cryomesh::manager::CryoManager> manager;

	boost::shared_ptr<display::DataWindow> dataWindow;
	boost::shared_ptr<display::StatisticsWindow> statisticsWindow;
	//TODO Structure window
	boost::shared_ptr<display::StructureWindow> structureWindow;
	boost::shared_ptr<display::OverviewWindow> overviewWindow;

};

}
}
#endif /* MAINWINDOW_H_ */
