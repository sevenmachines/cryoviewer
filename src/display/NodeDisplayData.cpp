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

using namespace cryomesh::components;

NodeDisplayData::NodeDisplayData(const Node * nd) : node(nd) , nodeSphere( new osg::Sphere(osg::Vec3(0,0,0), 1.0f)){
	this->update();
	nodeSphere->setName(node->getUUIDString());
}

NodeDisplayData::~NodeDisplayData() {
	const cryomesh::spacial::Point & pos = node->getPosition();
	nodeSphere->setCenter( osg::Vec3f(pos.getX(), pos.getY(), pos.getZ()));
}

void NodeDisplayData::update(){
	std::cout<<"NodeDisplayData::update: "<<""<<std::endl;
}

osg::ShapeDrawable * NodeDisplayData::getDrawable(){
	osg::ShapeDrawable* nodeDrawable = new osg::ShapeDrawable(nodeSphere);
	return nodeDrawable;
}
}//NAMESPACE

}//NAMESPACE

}//NAMESPACE
