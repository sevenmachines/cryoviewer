/*
 * NodeDisplayData.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ComponentDisplayData.h"

#ifndef NODEDISPLAYDATA_H_
#define NODEDISPLAYDATA_H_

namespace cryo {

namespace viewer {

namespace display {

class NodeDisplayData {
public:
	NodeDisplayData();
	virtual ~NodeDisplayData();
	virtual void update();

private:
	osg::Sphere * nodeSphere;
};


}

}

}

#endif /* NODEDISPLAYDATA_H_ */
