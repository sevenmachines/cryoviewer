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

NodeDisplayData::NodeDisplayData(const boost::shared_ptr<cryomesh::components::Node> nd) :
		node(nd), nodeSphere(
				boost::shared_ptr < elements::NodeSphere > (new elements::NodeSphere(osg::Vec3(0, 0, 0), 1.0f))) {
	nodeSphere->setName(node->getUUIDString());
	this->update();
}

NodeDisplayData::~NodeDisplayData() {
}

void NodeDisplayData::update() {
	const cryomesh::spacial::Point & pos = node->getPosition();
	nodeSphere->setCenter(osg::Vec3f(pos.getX(), pos.getY(), pos.getZ()));
}

boost::shared_ptr<osg::Shape> NodeDisplayData::getShape() {
	return nodeSphere;
}
} //NAMESPACE

} //NAMESPACE

} //NAMESPACE
