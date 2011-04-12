/*
 * StructureWindow.cpp
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "StructureWindow.h"

namespace cryo {

namespace viewer {

namespace display {

StructureWindow::StructureWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
	bundle(bun) {
	std::cout<<"StructureWindow::StructureWindow: "<<""<<std::endl;
	loadWindow("Data/structurewindow.ui");
}

StructureWindow::~StructureWindow() {
	// TODO Auto-generated destructor stub
}

void StructureWindow::updateData() {
	std::cout<<"StructureWindow::updateData: "<<""<<std::endl;
	std::cout << "StructureWindow::updateData: " << "" << std::endl;
}
void StructureWindow::initialise() {
	std::cout<<"StructureWindow::initialise: "<<""<<std::endl;
	std::cout << "StructureWindow::initialise: " << "" << std::endl;
}

}//NAMESPACE

}//NAMESPACE

}
