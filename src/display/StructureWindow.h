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
#include "components/NodeMap.h"
#include <gtkmm/button.h>
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

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);

	void setLabel(const std::string& name);

protected:

	Gtk::Button * statisticsVisualiseButton;

	void onStatisticsVisualiseButtonClicked();

	virtual void updateData();
	virtual void initialise();

	void showVisual(const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Node> > & nodes)  ;
	void showText(osg::Group* root) const ;

	osg::Node* createHUD(osgText::Text* updateText);

	void addNodeDrawable(const boost::shared_ptr<cryomesh::components::Node> & node, osg::Group* root) const ;

private:
	const boost::shared_ptr<cryomesh::structures::Bundle> bundle;
	boost::shared_ptr<cryomesh::components::Node> selectedNode;
	osg::ref_ptr<osgText::Text>  displayText;
};

}

}

}

#endif /* STRUCTUREWINDOW_H_ */
