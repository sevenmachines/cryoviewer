/*
 * ConnectionActivityPanel.h
 *
 *  Created on: 26 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONNECTIONACTIVITYPANEL_H_
#define CONNECTIONACTIVITYPANEL_H_

#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include "components/Connection.h"
#include <boost/shared_ptr.hpp>
#include "ConnectionActivityDrawingArea.h"

namespace cryo {

namespace viewer {

namespace display {

class ConnectionActivityPanel: public Gtk::HBox {
public:
	ConnectionActivityPanel(const boost::shared_ptr<cryomesh::components::Connection> & con);
	virtual ~ConnectionActivityPanel();

	const boost::shared_ptr<cryomesh::components::Connection> & getComponent() const;
	bool isActivated() const;
	void setActivated(bool b);
	void update();

protected:
	boost::shared_ptr<ConnectionActivityDrawingArea> activityDrawingArea;
	boost::shared_ptr<Gtk::CheckButton> activityCheckButton;
	boost::shared_ptr<Gtk::VBox> checkButtonsVBox;
	boost::shared_ptr<Gtk::Label> checkButtonsVBoxLabel;

	boost::shared_ptr<Gtk::Label> activityLabel;

	void initialise();
	void onActivityCheckButtonClicked();
	void setAsPrimaryInput();
			void setAsPrimaryOutput();
	static int ids;
	static int getIds();

private:
	const boost::shared_ptr<cryomesh::components::Connection> & component;
	bool activated;
	int id;

};

}

}

}

#endif /* CONNECTIONACTIVITYPANEL_H_ */
