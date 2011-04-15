/*
 * NodeActivityDrawingAreaPanel.cpp
 *
 *  Created on: 15 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "NodeActivityDrawingAreaPanel.h"

namespace cryo {

namespace viewer {

namespace display {

NodeActivityDrawingAreaPanel::NodeActivityDrawingAreaPanel(const boost::shared_ptr<cryomesh::components::Node> & nd) :
	node(nd), activated(false) {
	this->initialise();
}

NodeActivityDrawingAreaPanel::~NodeActivityDrawingAreaPanel() {
}
bool NodeActivityDrawingAreaPanel::isActivated() const {
	return activated;
}

void NodeActivityDrawingAreaPanel::setActivated(bool b) {
	activated = b;
	activityDrawingArea->showDrawingArea(this->isActivated());
}

void NodeActivityDrawingAreaPanel::update() {
	std::cout<<"NodeActivityDrawingAreaPanel::update: "<<""<<std::endl;
	if (this->isActivated() == true) {
		std::cout<<"NodeActivityDrawingAreaPanel::update: "<<"isActivated()"<<std::endl;
		activityDrawingArea->update();
	}
}

void NodeActivityDrawingAreaPanel::initialise() {
	std::cout << "NodeActivityDrawingAreaPanel::initialise: " << "" << std::endl;
	activityDrawingArea = boost::shared_ptr<NodeActivityDrawingArea>(new NodeActivityDrawingArea(node));
	activityCheckButton = boost::shared_ptr<Gtk::CheckButton>(new Gtk::CheckButton);

	activityCheckButton->signal_clicked().connect(
			sigc::mem_fun(*this, &NodeActivityDrawingAreaPanel::onActivityCheckButtonClicked));

	activityLabel = boost::shared_ptr<Gtk::Label>(new Gtk::Label(node->getUUIDString()));
	this->set_size_request(400, 100);
	//this->pack_start(*activityLabel);
	this->pack_start(*(activityCheckButton), false, false);
	this->pack_start(*(activityDrawingArea), true, true);
	this->show();
}

void NodeActivityDrawingAreaPanel::onActivityCheckButtonClicked() {
	if (activityCheckButton->get_active() == true) {
		this->setActivated(true);
		activityDrawingArea->setActivated(true);
	} else {
		this->setActivated(false);
		activityDrawingArea->setActivated(false);
	}
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
