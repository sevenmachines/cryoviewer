/*
 * StructureWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef STRUCTUREWINDOW_H_
#define STRUCTUREWINDOW_H_

#include "DisplayWindow.h"
#include "structures/Bundle.h"
#include "structures/Cluster.h"
#include "components/NodeMap.h"
#include "ActivitiesWindow.h"

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxtext.h>

#include <string>

namespace cryo {

namespace viewer {

namespace display {

class StructureWindow: public DisplayWindow {
public:
	StructureWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun);
	virtual ~StructureWindow();

	void setLabel(const std::string& name);

protected:

	//Tree model columns:
	class UUIDColumns: public Gtk::TreeModel::ColumnRecord {
	public:

		UUIDColumns() {
			add(columnID);
			add(columnName);
		}

		Gtk::TreeModelColumn<int> columnID; //The data to choose - this must be text.
		Gtk::TreeModelColumn<Glib::ustring> columnName;
	};

	UUIDColumns uuidColumns;

	//Child widgets:
	Glib::RefPtr<Gtk::ListStore> structureChooserClusterListStore;
	Gtk::ComboBox * structureChooserClusterComboBox;
	//Gtk::ComboBoxText structureChooserClusterEntries;

	Gtk::Button * structureVisualiseButton;
	Gtk::VBox * structureVBox;
	Gtk::ToggleButton * structureActivitiesToggleButton;
#ifdef STRUCTUREWINDOW_ENABLE_OPENGL
	StructureGLDrawingArea * structureDrawingArea;
#endif
	boost::shared_ptr<ActivitiesWindow> currentActivitiesWindow;
	//Child widgets:

	void onStructureVisualiseButtonClicked();
	void onStructureActivitiesToggleButtonClicked();
	void onStructureChooserClusterComboBoxChanged();

	void updateClusterChooser();
		virtual void setCluster(boost::shared_ptr<cryomesh::structures::Cluster>, bool activated=false);
	virtual void updateData();
	virtual void initialise();

	bool getPreserveActivityWindows()const;
	void setPreserveActivityWindows(bool b);

private:
	/**
	 * Save activity windows across switchings
	 */
	bool preserveActivityWindows;
	/**
	 * All activities windows mapped to their clusters
	 */
	std::map<boost::uuids::uuid, boost::shared_ptr< ActivitiesWindow > > preservedActivitiesWindows;

	const boost::shared_ptr<cryomesh::structures::Bundle> bundle;
	boost::shared_ptr<cryomesh::components::Node> selectedNode;
	boost::shared_ptr<cryomesh::structures::Cluster> selectedCluster;

};

}

}

}

#endif /* STRUCTUREWINDOW_H_ */
