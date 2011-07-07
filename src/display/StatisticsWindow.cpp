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

const int StatisticsWindow::MAX_LINE_COUNT = 10000;

StatisticsWindow::StatisticsWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
	bundle(bun) {
	std::cout<<"StatisticsWindow::StatisticsWindow: "<<"DEBUG START"<<std::endl;
loadWindow("Data/statisticswindow.glade");
	this->setTitle("Cryoviewer Statistics");
	std::cout<<"StatisticsWindow::StatisticsWindow: "<<"DEBUG END"<<std::endl;
}

StatisticsWindow::~StatisticsWindow() {
}

void StatisticsWindow::updateData() {
	std::cout << "StatisticsWindow::updateData: " << "" << std::endl;
	std::stringstream ss;
	ss<<*bundle;
	std::cout<<"StatisticsWindow::updateData: "<<statisticsTextBuffer->get_line_count()<<std::endl;
	ss << statisticsTextBuffer->insert(statisticsTextBuffer->begin(), ss.str());
	// chop to max line count'
	if (statisticsTextBuffer->get_line_count() > StatisticsWindow::MAX_LINE_COUNT) {
		std::cout<<"StatisticsWindow::updateData: "<<"DEBUG: Erasing some lines"<<std::endl;
		statisticsTextBuffer->erase(statisticsTextBuffer->get_iter_at_line(StatisticsWindow::MAX_LINE_COUNT),
				statisticsTextBuffer->end());
	}
}

void StatisticsWindow::initialise() {
	builder->get_widget("statisticsTextView", statisticsTextView);
	statisticsTextBuffer = Gtk::TextBuffer::create();
	statisticsTextView->set_buffer(statisticsTextBuffer);
	std::cout << "StructureWindow::initialise: " << "" << std::endl;
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
