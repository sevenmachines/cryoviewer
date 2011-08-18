/*
 * OverviewWindow.cpp
 *
 *  Created on: 17 Aug 2011
 *      Author: niall
 */

#include "OverviewWindow.h"

using namespace cryomesh::spacial;

namespace cryo {
namespace viewer {
namespace display {

OverviewWindow::OverviewWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
		bundle(bun) {
	loadWindow("Data/overviewwindow.glade");
}

OverviewWindow::~OverviewWindow() {
}

void OverviewWindow::initialise() {
	std::cout << "OverviewWindow::initialise: " << "" << std::endl;
	builder->get_widget("overviewWindowBox", overviewWindowBox);
	builder->get_widget("overviewViewportEnergy", overviewViewportEnergy);

	energyGraph = boost::shared_ptr < GraphWindow > (new GraphWindow());

	overviewViewportEnergy->add(*energyGraph);
	energyGraph->show();
	this->mainWindow->set_size_request(100, -1);
}

void OverviewWindow::updateData() {
	energyGraph->add(Point(cryomesh::common::TimeKeeper::getTimeKeeper().getCycle().toLInt(), bundle->getEnergy(), 0));
}
} /* namespace display */
} /* namespace viewer */
} /* namespace cryo */
