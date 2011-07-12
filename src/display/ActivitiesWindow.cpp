/*
 * ActivitiesWindow.cpp
 *
 *  Created on: 15 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

//#define ACTIVITIESWINDOW_DEBUG

#include "ActivitiesWindow.h"
#include <sstream>

using namespace cryomesh::components;

namespace cryo {
namespace viewer {
namespace display {

ActivitiesWindow::ActivitiesWindow(const boost::shared_ptr<cryomesh::structures::Cluster> clus) :
	cluster(clus) {
	loadWindow("Data/activitieswindow.glade");

	// set title
	{
		std::stringstream ss;
		ss << "Cluster: " << clus->getUUIDString();
		this->setTitle(ss.str());
	}

	this->initialise();
	mainWindow->show_all();
}

ActivitiesWindow::~ActivitiesWindow() {
}

void ActivitiesWindow::updateData() {
#ifdef ACTIVITIESWINDOW_DEBUG
	std::cout << "ActivitiesWindow::updateData: " << "Panels: " << drawingAreas.size() << std::endl;
#endif
	// forall in drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				drawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = drawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			it_drawingAreas->second->update();
			++it_drawingAreas;
		}
	}

	// forall in pin drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryInputDrawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = primaryInputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			it_drawingAreas->second->update();
			++it_drawingAreas;
		}
	}

	// forall in pout drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryOutputDrawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = primaryOutputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			it_drawingAreas->second->update();
			++it_drawingAreas;
		}
	}

	std::stringstream ss;
	ss << "Cycle: " << cryomesh::common::TimeKeeper::getTimeKeeper().getCycle();
	activitiesWindowLabelSummary->set_text(ss.str());
}

void ActivitiesWindow::initialise() {
	builder->get_widget("activitiesWindowHBox", activitiesWindowHBox);
	builder->get_widget("activitiesWindowCheckButtonSelectAll", activitiesWindowCheckButtonSelectAll);
	builder->get_widget("activitiesWindowLabelSummary", activitiesWindowLabelSummary);
	builder->get_widget("activitiesPrimaryInputsDrawingAreasVBox", activitiesPrimaryInputsDrawingAreasVBox);
	builder->get_widget("activitiesPrimaryOutputsDrawingAreasVBox", activitiesPrimaryOutputsDrawingAreasVBox);
	builder->get_widget("activitiesDrawingAreasVBox", activitiesDrawingAreasVBox);

	activitiesWindowCheckButtonSelectAll->signal_clicked().connect(
			sigc::mem_fun(*this, &ActivitiesWindow::onActivitiesWindowCheckButtonSelectAll));

	mainWindow->set_size_request(400, 300);
	this->updateNodeDisplay();
}

void ActivitiesWindow::updateNodeDisplay() {
	//std::cout << "ActivitiesWindow::updateNodeDisplay: " << "drawingAreas before: " << drawingAreas.size() << std::endl;
	std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> > drawing_areas_copy(drawingAreas);

	int count_added = 0;

	const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & all_nodes = cluster->getNodes();
	// forall in all_nodes
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes = all_nodes.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes_end = all_nodes.end();
		while (it_all_nodes != it_all_nodes_end) {
			boost::shared_ptr<NodeActivityDrawingAreaPanel> found_panel = this->findNodePanelByNode(
					it_all_nodes->second);

			//if node doesnt exist then add it
			if (found_panel == 0) {

				boost::shared_ptr<NodeActivityDrawingAreaPanel> panel = this->addNode(it_all_nodes->second);

				++count_added;
			} else {
				// if it does then delete it from the copy of drawing areas
				drawing_areas_copy.erase(it_all_nodes->first);
			}
			++it_all_nodes;
		}
	}

	int count_removed = 0;
	// remove all panels that werent in the cluster node collection
	// forall in drawing_areas_copy
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawing_areas_copy = drawing_areas_copy.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawing_areas_copy_end = drawing_areas_copy.end();
		while (it_drawing_areas_copy != it_drawing_areas_copy_end) {
			this->removeNode(it_drawing_areas_copy->first);
			++count_removed;
			++it_drawing_areas_copy;
		}
	}

	// check our panels and nodes balance out
#ifdef ACTIVITIESWINDOW_DEBUG
	std::cout << "ActivitiesWindow::updateNodeDisplay: " << "added: " << count_added << " removed: " << count_removed
			<< " total: " << drawingAreas.size() << std::endl;
	std::cout << "ActivitiesWindow::updateNodeDisplay: " << "drawingAreas after: " << drawingAreas.size() << std::endl;
#endif
	if (this->isActive() == true) {
		mainWindow->show_all();
	}
	if (this->isDebugOn() == true) {
		if (all_nodes.size() != drawingAreas.size()) {
			std::cout << "ActivitiesWindow::updateNodeDisplay: " << "ERROR: all_nodes.size() != drawingAreas.size(): "
					<< all_nodes.size() << " != " << drawingAreas.size() << std::endl;
			assert(false);
		}
	}
}

boost::shared_ptr<NodeActivityDrawingAreaPanel> ActivitiesWindow::addNode(
		const boost::shared_ptr<cryomesh::components::Node> & node) {
	boost::shared_ptr<NodeActivityDrawingAreaPanel> panel(new NodeActivityDrawingAreaPanel(node));
	bool is_pin = node->isPrimaryInputAttachedNode();
	bool is_pout = node->isPrimaryOutputAttachedNode();

	if (is_pin == true) {
		primaryInputDrawingAreas[node->getUUID()] = panel;
		activitiesPrimaryInputsDrawingAreasVBox->pack_start(*panel);
	}
	if (is_pout == true) {
		primaryOutputDrawingAreas[node->getUUID()] = panel;
		activitiesPrimaryOutputsDrawingAreasVBox->pack_start(*panel);
	}
	if (is_pin != true && is_pout != true) {
		drawingAreas[node->getUUID()] = panel;
		activitiesDrawingAreasVBox->pack_start(*panel);
	}
	return panel;
}

void ActivitiesWindow::removeNode(const boost::uuids::uuid & uuid) {
#ifdef ACTIVITIESWINDOW_DEBUG

	int pre_da_sz = drawingAreas.size();
	int pre_pin_da_sz = primaryInputDrawingAreas.size();
	int pre_pount_da_sz = primaryOutputDrawingAreas.size();
#endif
	bool id_found_dbg = false;
	bool is_pin_dbg = false;
	bool is_pout_dbg = false;

	// check remove from output primaries
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_pout_found =
				primaryOutputDrawingAreas.find(uuid);
		if (it_pout_found != primaryOutputDrawingAreas.end()) {
			boost::shared_ptr<NodeActivityDrawingAreaPanel> pout_found = it_pout_found->second;
			primaryOutputDrawingAreas.erase(it_pout_found);
			is_pout_dbg = true;
			activitiesPrimaryOutputsDrawingAreasVBox->remove(*pout_found);
		}
	}

	// check remove from input primaries
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_pin_found =
				primaryInputDrawingAreas.find(uuid);
		if (it_pin_found != primaryInputDrawingAreas.end()) {
			boost::shared_ptr<NodeActivityDrawingAreaPanel> pin_found = it_pin_found->second;
			primaryInputDrawingAreas.erase(it_pin_found);
			is_pin_dbg = true;
			activitiesPrimaryInputsDrawingAreasVBox->remove(*pin_found);
		}
	}

	if (is_pin_dbg != true && is_pout_dbg != true) {
		// remove from general
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_found =
					drawingAreas.find(uuid);
			if (it_found != drawingAreas.end()) {
				boost::shared_ptr<NodeActivityDrawingAreaPanel> found = it_found->second;
				drawingAreas.erase(it_found);
				id_found_dbg = true;
				activitiesDrawingAreasVBox->remove(*found);
			}
		}
	}

#ifdef ACTIVITIESWINDOW_DEBUG
	int post_da_sz = drawingAreas.size();
	int post_pin_da_sz = primaryInputDrawingAreas.size();
	int post_pount_da_sz = primaryOutputDrawingAreas.size();

	if (id_found_dbg == true) {
		assert(post_da_sz == pre_da_sz - 1);
	} else {
		assert(post_da_sz == pre_da_sz);
	}

	if (is_pin_dbg == true) {
		assert(post_pin_da_sz == pre_pin_da_sz - 1);
	} else {
		assert(post_pin_da_sz == pre_pin_da_sz );
	}
	if (is_pout_dbg == true) {
		assert(post_pount_da_sz == pre_pount_da_sz - 1);
	} else {
		assert(post_pount_da_sz == pre_pount_da_sz );
	}
#endif
}

void ActivitiesWindow::removeNode(const boost::shared_ptr<cryomesh::components::Node> &node) {
	this->removeNode(node->getUUID());
}

boost::shared_ptr<NodeActivityDrawingAreaPanel> ActivitiesWindow::findNodePanelByNode(
		const boost::shared_ptr<Node> & node) {
	boost::shared_ptr<NodeActivityDrawingAreaPanel> found_panel;
	bool found = false;
	const boost::uuids::uuid searchid = node->getUUID();
	// forall in drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				drawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = drawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end && found == false) {
			if (it_drawingAreas->second->getNode()->getUUID() == searchid) {
				found = true;
				found_panel = it_drawingAreas->second;
			}
			++it_drawingAreas;
		}
	}
	return found_panel;
}

void ActivitiesWindow::onActivitiesWindowCheckButtonSelectAll() {
	this->selectAllNodes(activitiesWindowCheckButtonSelectAll->get_active());
}

void ActivitiesWindow::selectAllNodes(bool b) {
	// forall in drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				drawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = drawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			it_drawingAreas->second->setActivated(b);
			++it_drawingAreas;
		}
	}

	// forall in pin drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryInputDrawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = primaryInputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			it_drawingAreas->second->setActivated(b);
			++it_drawingAreas;
		}
	}

	// forall in pout drawingAreas
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryOutputDrawingAreas.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator
				it_drawingAreas_end = primaryOutputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			it_drawingAreas->second->setActivated(b);
			++it_drawingAreas;
		}
	}

}
}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
