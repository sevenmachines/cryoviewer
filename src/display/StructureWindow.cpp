/*
 * StructureWindow.cpp
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "NodeDisplayData.h"
#include "StructureWindow.h"
#include <boost/uuid/uuid_io.hpp>

using namespace cryomesh::components;
using namespace cryomesh::structures;

namespace cryo {

namespace viewer {

namespace display {

StructureWindow::StructureWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
		bundle(bun) {
	loadWindow("Data/structurewindow.glade");
}

StructureWindow::~StructureWindow() {
}

void StructureWindow::updateData() {
#ifdef STRUCTUREWINDOW_ENABLE_OPENGL
	if (structureDrawingArea != 0) {
		structureDrawingArea->update();
	}
#endif
	if (activitiesWindow != 0) {
		activitiesWindow->update();
	}

}
void StructureWindow::setCluster(boost::shared_ptr<cryomesh::structures::Cluster> cluster, bool activated) {
	if (activitiesWindow != 0) {
		activitiesWindow->deactivate();
		activitiesWindow.reset();
	}
	selectedCluster = cluster;
	this->update();
	activitiesWindow = boost::shared_ptr < ActivitiesWindow > (new ActivitiesWindow(selectedCluster));
	structureActivitiesToggleButton->set_active(activated);
	if (activated == true) {
		activitiesWindow->activate();
	} else {
		activitiesWindow->deactivate();
	}
}

void StructureWindow::initialise() {
// get out widgets
	builder->get_widget("structureVisualiseButton", structureVisualiseButton);
	builder->get_widget("structureVBox", structureVBox);
	builder->get_widget("structureActivitiesToggleButton", structureActivitiesToggleButton);
	builder->get_widget("structureChooserClusterComboBox", structureChooserClusterComboBox);
	{
		//Create the Tree model:
		structureChooserClusterListStore = Gtk::ListStore::create(uuidColumns);
		structureChooserClusterComboBox->set_model(structureChooserClusterListStore);
		this->updateClusterChooser();
		structureChooserClusterComboBox->pack_start(uuidColumns.columnID);
		structureChooserClusterComboBox->pack_start(uuidColumns.columnName);
		//structureChooserClusterComboBox->set_entry_text_column(uuidColumns.columnID);

	}
	// connect up togglebuttons
	structureVisualiseButton->signal_clicked().connect(
			sigc::mem_fun(*this, &StructureWindow::onStructureVisualiseButtonClicked));
	structureActivitiesToggleButton->signal_clicked().connect(
			sigc::mem_fun(*this, &StructureWindow::onStructureActivitiesToggleButtonClicked));

	// create combo box xhooser
	//structureChooserClusterComboBox->add(structureChooserClusterEntries);
	structureChooserClusterComboBox->signal_changed().connect(
			sigc::mem_fun(*this, &StructureWindow::onStructureChooserClusterComboBoxChanged));

	// check if we have any clusters and set combo box if we do
	//structureChooserClusterComboBox->set_active(1);

#ifdef STRUCTUREWINDOW_ENABLE_OPENGL
	structureDrawingArea = new StructureGLDrawingArea(bundle);
	//TODO test by setting to first cluster
	structureDrawingArea->setDrawTest();

	structureVBox->pack_start(*structureDrawingArea);
#endif
//this->setCluster(bundle->getClusters().begin()->second);
	structureVBox->show();
}

void StructureWindow::updateClusterChooser() {
	if (structureChooserClusterComboBox != 0) {
		if (bundle != 0) {
			const std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> > & all_clusters =
					bundle->getClusters().getCollection();
			structureChooserClusterListStore->clear();
			// forall in all_clusters
			{
				int count = 1;
				std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> >::const_iterator it_all_clusters =
						all_clusters.begin();
				const std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> >::const_iterator it_all_clusters_end =
						all_clusters.end();
				Gtk::TreeModel::Row row;
				while (it_all_clusters != it_all_clusters_end) {
					row = *(structureChooserClusterListStore->append());
					row[uuidColumns.columnID] = count;
					row[uuidColumns.columnName] = it_all_clusters->second->getUUIDString();
					//uuidColumns.columnUUID = it_all_clusters->second->getUUID();
					std::cout << "StructureWindow::updateClusterChooser: " << "Adding: ID: " << count << " Name: "
							<< it_all_clusters->second->getUUIDString() << std::endl;
					++count;
					++it_all_clusters;
				}
			}
		}
	}

}

void StructureWindow::onStructureVisualiseButtonClicked() {
	std::cout << "StructureWindow::onStructureVisualiseButtonClicked: " << "TODO: This is a demo" << std::endl;
}

void StructureWindow::onStructureActivitiesToggleButtonClicked() {
	if (structureActivitiesToggleButton->get_active() == true) {
		if (activitiesWindow == 0) {
			activitiesWindow = boost::shared_ptr < ActivitiesWindow > (new ActivitiesWindow(selectedCluster));
		}
		activitiesWindow->activate();
	} else {
		activitiesWindow->deactivate();
	}
}

void StructureWindow::onStructureChooserClusterComboBoxChanged() {
	Gtk::TreeModel::iterator iter = structureChooserClusterComboBox->get_active();
	if (iter) {
		Gtk::TreeModel::Row row = *iter;
		if (row) {
			//Get the data for the selected row, using our knowledge of the tree
			//model:
			Glib::ustring name = row[uuidColumns.columnName];

			boost::shared_ptr < Cluster > found_cluster;
			// retreive a uuid
			{
				boost::uuids::string_generator gen;
				std::stringstream ss;
				ss << "{" << name << "}";
				boost::uuids::uuid temp_uuid = gen(ss.str());
				found_cluster = bundle->getClusters().getObjectByKey(temp_uuid);
			}
			//std::cout << " ID=" << id << ", name=" << name << std::endl;

			if (found_cluster != 0) {
				std::cout << "StructureWindow::onStructureChooserClusterComboBoxChanged: "
						<< "INFO: Setting active cluster to '" << found_cluster->getUUIDString() << "'" << std::endl;
				this->setCluster(found_cluster, true);
			} else {
				std::cout << "StructureWindow::onStructureChooserClusterComboBoxChanged: "
						<< "WARNING: Cannot find cluster: " << name << std::endl;
			}
		}
	}
}

} //NAMESPACE

} //NAMESPACE

}
