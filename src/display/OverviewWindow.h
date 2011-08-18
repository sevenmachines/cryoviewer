/*
 * OverviewWindow.h
 *
 *  Created on: 17 Aug 2011
 *      Author: niall
 */

#ifndef OVERVIEWWINDOW_H_
#define OVERVIEWWINDOW_H_

#include "DisplayWindow.h"
#include "GraphWindow.h"
#include "structures/Bundle.h"
#include "gtkmm/box.h"
#include <gtkmm/viewport.h>

#include <boost/shared_ptr.hpp>

namespace cryo {
namespace viewer {
namespace display {

class OverviewWindow : public DisplayWindow{
public:
	OverviewWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun);
	virtual ~OverviewWindow();

	virtual void updateData();
	virtual void initialise();

protected:
	Gtk::Box * overviewWindowBox;
	Gtk::Viewport * overviewViewportEnergy;
	boost::shared_ptr<GraphWindow> energyGraph;

private:
	const boost::shared_ptr<cryomesh::structures::Bundle> bundle;
};

} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
#endif /* OVERVIEWWINDOW_H_ */
