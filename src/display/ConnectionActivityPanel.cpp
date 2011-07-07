/*
 * ConnectionActivityPanel.cpp
 *
 *  Created on: 26 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ConnectionActivityPanel.h"
#include <gtkmm/label.h>

using namespace cryomesh::components;

namespace cryo {

namespace viewer {

namespace display {

int ConnectionActivityPanel::ids = 1;

int ConnectionActivityPanel::getIds() {
	int new_id = ConnectionActivityPanel::ids;
	++ConnectionActivityPanel::ids;
	return new_id;
}

ConnectionActivityPanel::ConnectionActivityPanel(const boost::shared_ptr<cryomesh::components::Connection> & con) :
		component(con), activated(false), id(ConnectionActivityPanel::getIds()) {
	this->initialise();
}

ConnectionActivityPanel::~ConnectionActivityPanel() {
}
bool ConnectionActivityPanel::isActivated() const {
	return activated;
}

void ConnectionActivityPanel::setActivated(bool b) {
	activated = b;
	//activityDrawingArea->showDrawingArea(this->isActivated());
	activityDrawingArea->setActivated(b);
	activityCheckButton->set_active(b);
}

void ConnectionActivityPanel::update() {
	if (this->isActivated() == true) {
		activityDrawingArea->update();

		std::cout << "ConnectionActivityPanel::update: " << *component << std::endl;
	}
}

void ConnectionActivityPanel::initialise() {
	activityDrawingArea = boost::shared_ptr<ConnectionActivityDrawingArea>(new ConnectionActivityDrawingArea(component));
	// setup label
	{
		std::stringstream ss;
		ss << this->id;
		checkButtonsVBoxLabel = boost::shared_ptr<Gtk::Label>(new Gtk::Label(ss.str()));

	}

	// setup check buttons
	{
		checkButtonsVBox = boost::shared_ptr<Gtk::VBox>(new Gtk::VBox);

		activityCheckButton = boost::shared_ptr<Gtk::CheckButton>(new Gtk::CheckButton("Activated"));
		activityCheckButton->signal_clicked().connect(
				sigc::mem_fun(*this, &ConnectionActivityPanel::onActivityCheckButtonClicked));


	}

	// create checkbutton layout
	{
		checkButtonsVBox->pack_start(*(checkButtonsVBoxLabel), false, false);
		checkButtonsVBox->pack_start(*(activityCheckButton), false, false);
	}


	activityLabel = boost::shared_ptr<Gtk::Label>(new Gtk::Label(component->getUUIDString()));
	if (component->isPrimaryInputConnection() == true) {
		this->setAsPrimaryInput();
	} else if (component->isPrimaryOutputConnection() == true) {
		this->setAsPrimaryOutput();
	}
	this->set_size_request(400, 100);
	//this->pack_start(*activityLabel);
	this->pack_start(*checkButtonsVBox, false, false);
	this->pack_start(*(activityDrawingArea), true, true);
	this->show();
}

void ConnectionActivityPanel::setAsPrimaryInput() {
}

void ConnectionActivityPanel::setAsPrimaryOutput() {
}

void ConnectionActivityPanel::onActivityCheckButtonClicked() {
	//std::cout << "ConnectionActivityPanel::onActivityCheckButtonClicked: " << "" << std::endl;
	if (activityCheckButton->get_active() == true) {
		//std::cout << "ConnectionActivityPanel::onActivityCheckButtonClicked: " << "TRUE" << std::endl;
		component->enableDebug(true);
		this->setActivated(true);
		activityDrawingArea->setActivated(true);
		//activityDrawingArea->invalidateWindow();
	} else {
		component->enableDebug(false);
		this->setActivated(false);
		activityDrawingArea->setActivated(false);
		//activityDrawingArea->invalidateWindow();
	}
}

const boost::shared_ptr<cryomesh::components::Connection> & ConnectionActivityPanel::getComponent() const{
	return component;
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
