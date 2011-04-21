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

int NodeActivityDrawingAreaPanel::ids = 1;

int NodeActivityDrawingAreaPanel::getIds() {
	int new_id = NodeActivityDrawingAreaPanel::ids;
	++NodeActivityDrawingAreaPanel::ids;
	return new_id;
}

NodeActivityDrawingAreaPanel::NodeActivityDrawingAreaPanel(const boost::shared_ptr<cryomesh::components::Node> & nd) :
	node(nd), activated(false) , id(NodeActivityDrawingAreaPanel::getIds()){
	this->initialise();
}

NodeActivityDrawingAreaPanel::~NodeActivityDrawingAreaPanel() {
}
bool NodeActivityDrawingAreaPanel::isActivated() const {
	return activated;
}

void NodeActivityDrawingAreaPanel::setActivated(bool b) {
	activated = b;
	//activityDrawingArea->showDrawingArea(this->isActivated());
	activityDrawingArea->setActivated(b);
	activityCheckButton->set_active(b);
}

void NodeActivityDrawingAreaPanel::update() {
	if (this->isActivated() == true) {
		activityDrawingArea->update();
		std::cout << "NodeActivityDrawingAreaPanel::update: " << *node << std::endl;
	}
}

void NodeActivityDrawingAreaPanel::initialise() {
	activityDrawingArea = boost::shared_ptr<NodeActivityDrawingArea>(new NodeActivityDrawingArea(node));
	std::stringstream ss;
	ss<<this->id;

	activityCheckButton = boost::shared_ptr<Gtk::CheckButton>(new Gtk::CheckButton(ss.str()));

	activityCheckButton->signal_clicked().connect(
			sigc::mem_fun(*this, &NodeActivityDrawingAreaPanel::onActivityCheckButtonClicked));

	activityLabel = boost::shared_ptr<Gtk::Label>(new Gtk::Label(node->getUUIDString()));
	this->set_size_request(400, 100);
	//this->pack_start(*activityLabel);
	this->pack_start(*(activityCheckButton), false, false);
	this->pack_start(*(activityDrawingArea), true, true);
	this->show();
}

void NodeActivityDrawingAreaPanel::setAsPrimaryInput(){
	activityDrawingArea->setAsPrimaryInput();
}

void NodeActivityDrawingAreaPanel::setAsPrimaryOutput(){
	activityDrawingArea->setAsPrimaryOutput();
}

void NodeActivityDrawingAreaPanel::onActivityCheckButtonClicked() {
	//std::cout << "NodeActivityDrawingAreaPanel::onActivityCheckButtonClicked: " << "" << std::endl;
	if (activityCheckButton->get_active() == true) {
		//std::cout << "NodeActivityDrawingAreaPanel::onActivityCheckButtonClicked: " << "TRUE" << std::endl;
		node->setDebug(true);
		this->setActivated(true);
		activityDrawingArea->setActivated(true);
		//activityDrawingArea->invalidateWindow();
	} else {
		node->setDebug(false);
		this->setActivated(false);
		activityDrawingArea->setActivated(false);
		//activityDrawingArea->invalidateWindow();
}
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
