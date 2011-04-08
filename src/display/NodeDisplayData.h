/*
 * NodeDisplayData.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ComponentDisplayData.h"
#include <components/Node.h>
#include <boost/shared_ptr.hpp>

#ifndef NODEDISPLAYDATA_H_
#define NODEDISPLAYDATA_H_

namespace cryo {

namespace viewer {

namespace display {

class NodeDisplayData : public ComponentDisplayData {
public:
	NodeDisplayData(const boost::shared_ptr< cryomesh::components::Node > nd);
	virtual ~NodeDisplayData();
	virtual void update();
	virtual boost::shared_ptr< osg::ShapeDrawable > getDrawable();

private:
	const boost::shared_ptr< cryomesh::components::Node  >node;
	boost::shared_ptr< osg::Sphere > nodeSphere;
};


}

}

}

#endif /* NODEDISPLAYDATA_H_ */
