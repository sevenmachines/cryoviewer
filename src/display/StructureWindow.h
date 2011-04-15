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
#include "StructureGLDrawingArea.h"
#include "ActivitiesWindow.h"

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/box.h>
#include <string>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgText/Text>
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

	Gtk::Button * structureVisualiseButton;
	Gtk::VBox * structureVBox;
	Gtk::ToggleButton * structureActivitiesToggleButton;
	StructureGLDrawingArea * structureDrawingArea;
	boost::shared_ptr< ActivitiesWindow > activitiesWindow;

	void onStructureVisualiseButtonClicked();
	void onStructureActivitiesToggleButtonClicked();

	virtual void updateData();
	virtual void initialise();

	void showVisual(const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Node> > & nodes)  ;
	void showText(osg::Group* root) const ;

	osg::Node* createHUD(osgText::Text* updateText);

	void addNodeDrawable(const boost::shared_ptr<cryomesh::components::Node> & node, osg::Group* root) const ;

private:
	const boost::shared_ptr<cryomesh::structures::Bundle> bundle;
	boost::shared_ptr<cryomesh::components::Node> selectedNode;
	boost::shared_ptr<cryomesh::structures::Cluster> selectedCluster;

	osg::ref_ptr<osgText::Text>  displayText;
};

}

}

}

#endif /* STRUCTUREWINDOW_H_ */
