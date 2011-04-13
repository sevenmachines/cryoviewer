/*
 * NodeSphere.h
 *
 *  Created on: 12 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef NODESPHERE_H_
#define NODESPHERE_H_

#include <osg/ShapeDrawable>

namespace cryo {

namespace viewer {

namespace elements {

class NodeSphere : public osg::Sphere{
public:
	NodeSphere(osg::Vec3 vec, float f);
	virtual ~NodeSphere();

private:
};

}

}

}

#endif /* NODESPHERE_H_ */
