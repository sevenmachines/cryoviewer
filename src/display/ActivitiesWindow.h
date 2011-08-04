/*
 * ActivitiesWindow.h
 *
 *  Created on: 15 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ACTIVITIESWINDOW_H_
#define ACTIVITIESWINDOW_H_

#include "DisplayWindow.h"
#include "NodeActivityDrawingAreaPanel.h"
#include "structures/Cluster.h"

#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <vector>

namespace cryo {

namespace viewer {

namespace display {

class ActivitiesWindow: public DisplayWindow {
public:
	ActivitiesWindow(const boost::shared_ptr<cryomesh::structures::Cluster> clus);
	virtual ~ActivitiesWindow();

protected:
	Gtk::VBox * activitiesPrimaryInputsDrawingAreasVBox;
	Gtk::VBox * activitiesPrimaryOutputsDrawingAreasVBox;
	Gtk::VBox * activitiesDrawingAreasVBox;
	Gtk::HBox * activitiesWindowHBox;
	Gtk::CheckButton * activitiesWindowCheckButtonSelectAll;
	Gtk::Label * activitiesWindowLabelSummary;

	virtual void updateData();
	virtual void initialise();

	virtual void updateNodeDisplay(bool force_recreate = false);

	virtual boost::shared_ptr<NodeActivityDrawingAreaPanel> addNode(
			const boost::shared_ptr<cryomesh::components::Node> & node);
	virtual void removeNode(const boost::uuids::uuid & uuid);
	virtual void removeNode(const boost::shared_ptr<cryomesh::components::Node> & node);
	boost::shared_ptr<NodeActivityDrawingAreaPanel> findNodePanelByNode(
			const boost::shared_ptr<cryomesh::components::Node> & node);

	void onActivitiesWindowCheckButtonSelectAll();

	void deleteAllChildren(Gtk::Container & container);
	void clearDeadPanels();

	std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> >::iterator findPanelByNodeUUID(const boost::uuids::uuid & uuid,
			std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> > & container);
private:
	const boost::shared_ptr<cryomesh::structures::Cluster> cluster;
	std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> > drawingAreas;
	std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> > primaryInputDrawingAreas;
	std::vector<boost::shared_ptr<NodeActivityDrawingAreaPanel> > primaryOutputDrawingAreas;

	void selectAllNodes(bool b);

};

} //NAMESPACE
} //NAMESPACE
} //NAMESPACE

#endif /* ACTIVITIESWINDOW_H_ */
