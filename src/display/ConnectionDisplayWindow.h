/*
 * ConnectionDisplayWindow.h
 *
 *  Created on: 26 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONNECTIONDISPLAYWINDOW_H_
#define CONNECTIONDISPLAYWINDOW_H_

#include "DisplayWindow.h"
#include "ConnectionActivityPanel.h"
#include "components/Node.h"
#include "components/Connection.h"
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <gtkmm/box.h>

namespace cryo {

namespace viewer {

namespace display {

/**
 * Window for displaying the input and output connections of a node
 */
class ConnectionDisplayWindow: public DisplayWindow {
public:
	enum ComponentType {
		INPUT_CONNECTIONS_PANEL, OUTPUT_CONNECTIONS_PANEL
	};
	ConnectionDisplayWindow(const boost::shared_ptr<cryomesh::components::Node> & nd);
	virtual ~ConnectionDisplayWindow();

protected:
	virtual void updateData();
	virtual void initialise();

	Gtk::VBox * connectionDisplayInputsVBox;
	Gtk::VBox * connectionDisplayOutputsVBox;
	Gtk::CheckButton * connectionDisplayInputsCheckBox;
	Gtk::CheckButton * connectionDisplayOutputsCheckBox;

	virtual void updateInputComponentDisplay();
	virtual void updateOutputComponentDisplay();
	virtual void updateComponentDisplay(
			const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> > & all_components,
			std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap,
			const ComponentType & type);

	boost::shared_ptr<ConnectionActivityPanel> addInputComponent(
			const boost::shared_ptr<cryomesh::components::Connection> & connection);

	boost::shared_ptr<ConnectionActivityPanel> addOutputComponent(
			const boost::shared_ptr<cryomesh::components::Connection> & connection);

	boost::shared_ptr<ConnectionActivityPanel> addComponent(
			const boost::shared_ptr<cryomesh::components::Connection> & connection,
			std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap);

	void removeInputComponent(const boost::uuids::uuid & uuid);

	void removeOutputComponent(const boost::uuids::uuid & uuid);

	boost::shared_ptr<ConnectionActivityPanel> removeComponent(const boost::uuids::uuid & uuid,
			std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap);

	boost::shared_ptr<ConnectionActivityPanel> findComponentPanelByUUID(
			const boost::shared_ptr<cryomesh::components::Connection> & component,
			std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap);

	virtual void onConnectionDisplayInputsCheckBoxClicked();
	virtual void onConnectionDisplayOutputsCheckBoxClicked();
	virtual void showAllConnections(std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> >  & connection_panels, bool b);

private:
	const boost::shared_ptr<cryomesh::components::Node> & node;
	std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > inputPanelsMap;
	std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > outputPanelsMap;
};

}

}

}

#endif /* CONNECTIONDISPLAYWINDOW_H_ */
