/*
 * NodeActivityDrawingAreaPanel.h
 *
 *  Created on: 15 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef NODEACTIVITYDRAWINGAREAPANEL_H_
#define NODEACTIVITYDRAWINGAREAPANEL_H_

#include "NodeActivityDrawingArea.h"

#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>
namespace cryo {

namespace viewer {

namespace display {

class NodeActivityDrawingAreaPanel : public Gtk::HBox{
public:
	NodeActivityDrawingAreaPanel(const boost::shared_ptr< cryomesh::components::Node > & nd);
	virtual ~NodeActivityDrawingAreaPanel();
	void setAsPrimaryInput();
	void setAsPrimaryOutput();
	bool isActivated()const;
	void setActivated(bool b);
	void update();
	const boost::shared_ptr< cryomesh::components::Node > & getNode()const{
		return node;
	}

protected:
	boost::shared_ptr< NodeActivityDrawingArea > activityDrawingArea;
	boost::shared_ptr< Gtk::CheckButton > activityCheckButton;
	boost::shared_ptr< Gtk::Label > activityLabel;

	void initialise();

	void onActivityCheckButtonClicked() ;

	static int ids;
	static int getIds();

private:
	const boost::shared_ptr< cryomesh::components::Node > node;
	bool activated;
	int id;
};

}

}

}

#endif /* NODEACTIVITYDRAWINGAREAPANEL_H_ */
