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
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas = drawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				drawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			(*it_drawingAreas)->update();
			++it_drawingAreas;
		}
	}

	// forall in pin drawingAreas
	{
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryInputDrawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				primaryInputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			(*it_drawingAreas)->update();
			++it_drawingAreas;
		}
	}

	// forall in pout drawingAreas
	{
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryOutputDrawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				primaryOutputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			(*it_drawingAreas)->update();
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
	this->updateNodeDisplay(true);
}

void ActivitiesWindow::updateNodeDisplay(bool force_recreate) {
#ifdef ACTIVITIESWINDOW_DEBUG
	std::cout << "ActivitiesWindow::updateNodeDisplay: " << "drawingAreas before: " << drawingAreas.size() << std::endl;
#endif
	if (force_recreate == true) {
#ifdef ACTIVITIESWINDOW_DEBUG
		unsigned int count_added = 0;
#endif
		drawingAreas.clear();
		primaryInputDrawingAreas.clear();
		primaryOutputDrawingAreas.clear();

		this->deleteAllChildren(*activitiesDrawingAreasVBox);
		this->deleteAllChildren(*activitiesPrimaryInputsDrawingAreasVBox);
		this->deleteAllChildren(*activitiesPrimaryOutputsDrawingAreasVBox);
		const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & all_nodes = cluster->getNodes();

		size_t node_overspill = all_nodes.size() +  std::min(static_cast<size_t>(10), (all_nodes.size() / 100) );

		drawingAreas.reserve( node_overspill );

		// forall in all_nodes
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes = all_nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes_end =
					all_nodes.end();
			while (it_all_nodes != it_all_nodes_end) {
				boost::shared_ptr<NodeActivityDrawingAreaPanel> panel = this->addNode(it_all_nodes->second);
#ifdef ACTIVITIESWINDOW_DEBUG
				boost::uuids::uuid panel_node_uuid = panel->getNode()->getUUID();
				std::cout << "ActivitiesWindow::updateNodeDisplay: " << "Forcing recreation: new panel ( "
						<< count_added << " of " << all_nodes.size() << " )" << " nodeid " << panel_node_uuid
						<< std::endl;
				assert(it_all_nodes->first == panel_node_uuid);
				++count_added;
#endif
				++it_all_nodes;
			}
#ifdef ACTIVITIESWINDOW_DEBUG
			assert(count_added == all_nodes.size());
			assert(all_nodes.size() != drawingAreas.size());
#endif
		}
	} else {
		int count_added = 0;

		const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & all_nodes = cluster->getNodes();
		// forall in all_nodes
		{
			std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes = all_nodes.begin();
			const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes_end =
					all_nodes.end();
			while (it_all_nodes != it_all_nodes_end) {
				boost::shared_ptr<NodeActivityDrawingAreaPanel> found_panel = this->findNodePanelByNode(
						it_all_nodes->second);

				//if panel node doesnt exist then add it
				if (found_panel == 0) {

					boost::shared_ptr<NodeActivityDrawingAreaPanel> panel = this->addNode(it_all_nodes->second);

					++count_added;
				}
				++it_all_nodes;
			}
		}

		this->clearDeadPanels();

		if (this->isActive() == true) {
			mainWindow->show_all();
		}
		assert(all_nodes.size() != drawingAreas.size());
	}

}

boost::shared_ptr<NodeActivityDrawingAreaPanel> ActivitiesWindow::addNode(
		const boost::shared_ptr<cryomesh::components::Node> & node) {
	boost::shared_ptr<NodeActivityDrawingAreaPanel> panel(new NodeActivityDrawingAreaPanel(node));
	bool is_pin = node->isPrimaryInputAttachedNode();
	bool is_pout = node->isPrimaryOutputAttachedNode();

	if (is_pin == true) {
		primaryInputDrawingAreas.push_back(panel);
		activitiesPrimaryInputsDrawingAreasVBox->pack_start(*panel);
#ifdef ACTIVITIESWINDOW_DEBUG
		std::cout << "ActivitiesWindow::addNode: " << "Adding primary input panel: " << panel->getNode()->getUUID()
				<< std::endl;
#endif
	}
	if (is_pout == true) {
		primaryOutputDrawingAreas.push_back(panel);
		activitiesPrimaryOutputsDrawingAreasVBox->pack_start(*panel);
#ifdef ACTIVITIESWINDOW_DEBUG
		std::cout << "ActivitiesWindow::addNode: " << "Adding primary output panel: " << panel->getNode()->getUUID()
				<< std::endl;
#endif
	}
	if (is_pin != true && is_pout != true) {
		drawingAreas.push_back(panel);
		activitiesDrawingAreasVBox->pack_start(*panel);
#ifdef ACTIVITIESWINDOW_DEBUG
		std::cout << "ActivitiesWindow::addNode: " << "Adding normal panel: " << panel->getNode()->getUUID()
				<< std::endl;
#endif
	}
	return panel;
}

void ActivitiesWindow::removeNode(const boost::uuids::uuid & uuid) {
#ifdef ACTIVITIESWINDOW_DEBUG
	bool id_found_dbg = false;
	int pre_da_sz = drawingAreas.size();
	int pre_pin_da_sz = primaryInputDrawingAreas.size();
	int pre_pount_da_sz = primaryOutputDrawingAreas.size();
#endif
	bool is_pin_dbg = false;
	bool is_pout_dbg = false;

	// check remove from output primaries
	{
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_pout_found =
				this->findPanelByNodeUUID(uuid, primaryOutputDrawingAreas);
		if (it_pout_found != primaryOutputDrawingAreas.end()) {
			boost::shared_ptr<NodeActivityDrawingAreaPanel> pout_found = *it_pout_found;
			primaryOutputDrawingAreas.erase(it_pout_found);
			is_pout_dbg = true;
			activitiesPrimaryOutputsDrawingAreasVBox->remove(*pout_found);
		}
	}

	// check remove from input primaries
	{
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_pin_found =
				this->findPanelByNodeUUID(uuid, primaryInputDrawingAreas);
		if (it_pin_found != primaryInputDrawingAreas.end()) {
			boost::shared_ptr<NodeActivityDrawingAreaPanel> pin_found = *it_pin_found;
			primaryInputDrawingAreas.erase(it_pin_found);
			is_pin_dbg = true;
			activitiesPrimaryInputsDrawingAreasVBox->remove(*pin_found);
		}
	}

	if (is_pin_dbg != true && is_pout_dbg != true) {
		// remove from general
		{
			std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_found =
					this->findPanelByNodeUUID(uuid, drawingAreas);
			if (it_found != drawingAreas.end()) {
#ifdef ACTIVITIESWINDOW_DEBUG
				id_found_dbg = true;
#endif
				boost::shared_ptr<NodeActivityDrawingAreaPanel> found = *it_found;
				drawingAreas.erase(it_found);
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
		assert(post_pin_da_sz == pre_pin_da_sz);
	}
	if (is_pout_dbg == true) {
		assert(post_pount_da_sz == pre_pount_da_sz - 1);
	} else {
		assert(post_pount_da_sz == pre_pount_da_sz);
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
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas = drawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				drawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end && found == false) {
			if ((*it_drawingAreas)->getNode()->getUUID() == searchid) {
				found = true;
				found_panel = *it_drawingAreas;
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
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas = drawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				drawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			(*it_drawingAreas)->setActivated(b);
			++it_drawingAreas;
		}
	}

	// forall in pin drawingAreas
	{
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryInputDrawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				primaryInputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			(*it_drawingAreas)->setActivated(b);
			++it_drawingAreas;
		}
	}

	// forall in pout drawingAreas
	{
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_drawingAreas =
				primaryOutputDrawingAreas.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_drawingAreas_end =
				primaryOutputDrawingAreas.end();
		while (it_drawingAreas != it_drawingAreas_end) {
			(*it_drawingAreas)->setActivated(b);
			++it_drawingAreas;
		}
	}

}

void ActivitiesWindow::deleteAllChildren(Gtk::Container & container) {
	std::vector<Gtk::Widget*> child_list = container.get_children();
	// forall in child_list
	{
		std::vector<Gtk::Widget*>::iterator it_child_list = child_list.begin();
		const std::vector<Gtk::Widget*>::const_iterator it_child_list_end = child_list.end();
		while (it_child_list != it_child_list_end) {
			container.remove(**it_child_list);
			++it_child_list;
		}
	}
	assert(container.get_children().size() == 0);

}

void ActivitiesWindow::clearDeadPanels() {
	std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> > & container = drawingAreas;
	// forall in drawingAreas
	{
		const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & all_nodes = cluster->getNodes();
		const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_node_end = all_nodes.end();

		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_container = container.begin();
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_container_end =
				container.end();

		while (it_container != it_container_end) {
			std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_found_node = all_nodes.find(
					(*it_container)->getNode()->getUUID());
			if (it_found_node == it_all_node_end) {
				this->removeNode(it_found_node->first);
				it_container_end = container.end();
			}
			if (it_container != it_container_end) {
				++it_container;
			}
		}
	}
}
std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator ActivitiesWindow::findPanelByNodeUUID(
		const boost::uuids::uuid & uuid, std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> > & container) {
	std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator found_panel;
	// forall in container
	{
		bool found = false;
		std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator it_container = container.begin();
		const std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::const_iterator it_container_end =
				container.end();
		while (found == false && (it_container != it_container_end)) {
			if ((*it_container)->getNode()->getUUID() == uuid) {
				found_panel = it_container;
				found = true;
			}
			++it_container;
		}
	}
	return found_panel;
}
} //NAMESPACE
} //NAMESPACE
} //NAMESPACE
