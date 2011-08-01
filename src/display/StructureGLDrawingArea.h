/*
 * StructureGLDrawingArea.h
 *
 *  Created on: 14 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef STRUCTUREGLDRAWINGAREA_H_
#define STRUCTUREGLDRAWINGAREA_H_

#include "components/Node.h"
#include "components/Connection.h"
#include "structures/Bundle.h"
#include "structures/Cluster.h"
#include "structures/Fibre.h"
#include "spacial/Point.h"
#include "common/SimpleGLDrawingArea.h"
#include <gtkmm/box.h>

using namespace cryomesh::structures;
using namespace cryomesh::components;

namespace cryo {

namespace viewer {

namespace display {

class StructureGLDrawingArea: public Gtk::VBox {
public:

	/**
	 * Enum representing drawmode
	 */
	enum DrawMode {
		DRAW_BUNDLE, DRAW_CLUSTER, DRAW_TEST, DRAW_NODE
	};

	StructureGLDrawingArea(const boost::shared_ptr<cryomesh::structures::Bundle> bun);
	virtual ~StructureGLDrawingArea();

	virtual void update();
	virtual void setDrawBundle();
	virtual void setDrawTest();
	virtual void setDrawCluster();
	virtual void setDrawNode();
	virtual void setCurrentBundle(boost::shared_ptr<const Bundle> bun);
	virtual void setCurrentCluster(boost::shared_ptr<const Cluster> clus);
	virtual void setCurrentNode(boost::shared_ptr<const Node> node);

protected:

	virtual void initialise();

	virtual void setDrawMode(const DrawMode mode);

	virtual void clear();
	virtual void drawBundle();
	virtual void drawCluster();
	virtual void drawNode();
	virtual void drawTest();

	virtual void invalidate();

	boost::shared_ptr<const Bundle> currentBundle;
	boost::shared_ptr<const Cluster> currentCluster;
	boost::shared_ptr<const Node> currentNode;

	DrawMode drawMode;
	boost::uuids::uuid drawUUID;

	boost::shared_ptr<common::SimpleGLDrawingArea> bundleDrawingArea;
	boost::shared_ptr<common::SimpleGLDrawingArea> clusterDrawingArea;
	boost::shared_ptr<common::SimpleGLDrawingArea> nodeDrawingArea;
	boost::shared_ptr<common::SimpleGLDrawingArea> testDrawingArea;

};

}

}

}

#endif /* STRUCTUREGLDRAWINGAREA_H_ */
