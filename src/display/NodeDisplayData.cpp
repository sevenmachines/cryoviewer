/*
 * NodeDisplayData.cpp
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "NodeDisplayData.h"
#include <iostream>

namespace cryo {

namespace viewer {

namespace display {

NodeDisplayData::NodeDisplayData() {
	nodeSphere = new osg::Sphere(osg::Vec3(0,0,0), 1.0f);

}

NodeDisplayData::~NodeDisplayData() {
	// TODO Auto-generated destructor stub
}

void NodeDisplayData::update(){
	std::cout<<"NodeDisplayData::update: "<<""<<std::endl;
}
}//NAMESPACE

}//NAMESPACE

}//NAMESPACE
