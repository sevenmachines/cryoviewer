/*
 * StatisticsWindow.cpp
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "StatisticsWindow.h"

namespace cryo {

namespace viewer {

namespace display {

StatisticsWindow::StatisticsWindow(const boost::shared_ptr< cryomesh::structures::Bundle > bun) : bundle(bun){
	loadWindow("Data/statisticswindow.ui");
	this->setTitle("Cryoviewer Statistics");
}

StatisticsWindow::~StatisticsWindow() {
}

void StatisticsWindow::updateData() {
	std::cout<<"StructureWindow::updateData: "<<""<<std::endl;
}
void StatisticsWindow::initialise() {
	std::cout<<"StructureWindow::initialise: "<<""<<std::endl;
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
