/*
 * StructureGLDrawingArea.cpp
 *
 *  Created on: 14 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "StructureGLDrawingArea.h"
#include "common/SimpleGLShape.h"
#include "structures/Cluster.h"
#include "structures/Fibre.h"
#include "components/Node.h"
#include "spacial/Point.h"

#include <GL/gl.h>

#include <GL/glu.h>
namespace cryo {

namespace viewer {

namespace display {

using namespace cryomesh::components;
using namespace cryomesh::structures;
using namespace cryomesh::common;
using namespace cryo::viewer::common;

using namespace cryomesh::spacial;
using namespace cryomesh;

StructureGLDrawingArea::StructureGLDrawingArea(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
	currentBundle(bun), drawMode(DRAW_TEST), bundleDrawingArea(new common::SimpleGLDrawingArea),
			clusterDrawingArea(new common::SimpleGLDrawingArea), nodeDrawingArea(new common::SimpleGLDrawingArea),
			testDrawingArea(new common::SimpleGLDrawingArea) {

	this->pack_start(*bundleDrawingArea);
	this->pack_start(*clusterDrawingArea);
	this->pack_start(*nodeDrawingArea);
	this->setDrawBundle();
	this->initialise();
	this->show();
}

StructureGLDrawingArea::~StructureGLDrawingArea() {
	// TODO Auto-generated destructor stub
}

void StructureGLDrawingArea::update() {
	if (drawMode == DrawMode::DRAW_BUNDLE) {
		if (currentBundle != 0) {
			// get all clusters
			{
				const std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> > & all_clusters =
						currentBundle->getClusters().getCollection();

				bundleDrawingArea->clear();
				// forall in all_clusters
				{
					std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> >::const_iterator it_all_clusters =
							all_clusters.begin();
					const std::map<boost::uuids::uuid, boost::shared_ptr<Cluster> >::const_iterator
							it_all_clusters_end = all_clusters.end();
					while (it_all_clusters != it_all_clusters_end) {
						Point centre = it_all_clusters->second->getPosition();
						Point radius(centre.getX() + 1, centre.getY(), centre.getZ());
						std::vector<Point> points;
						points .push_back(centre);
						points .push_back(radius);
						boost::shared_ptr<common::SimpleGLShape> temp_shape(
								new common::SimpleGLShape(common::SimpleGLShape::SPHERE, points));
						bundleDrawingArea->add(temp_shape);
						++it_all_clusters;
					}
				}
			}

			// TODO get all fibres
			{

			}
		} else {
			std::cout << "StructureGLDrawingArea::update: " << "ERROR: Not updating null bundle." << std::endl;
		}

	} else if (drawMode == DrawMode::DRAW_CLUSTER) {
		if (currentCluster != 0) {
			// get nodes
			{
				nodeDrawingArea->clear();
				const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & all_nodes = currentCluster->getNodes();
				// forall in all_nodes
				{
					std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes =
							all_nodes.begin();
					const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes_end =
							all_nodes.end();
					while (it_all_nodes != it_all_nodes_end) {
						Point centre = it_all_nodes->second->getPosition();
						Point radius(centre.getX() + 1, centre.getY(), centre.getZ());
						std::vector<Point> points;
						points .push_back(centre);
						points .push_back(radius);
						boost::shared_ptr<common::SimpleGLShape> temp_shape(
								new common::SimpleGLShape(common::SimpleGLShape::SPHERE, points));
						nodeDrawingArea->add(temp_shape);
						++it_all_nodes;
					}
				}
			}
		} else {
			std::cout << "StructureGLDrawingArea::update: " << "ERROR: Not updating null cluster." << std::endl;
		}
	}
}

void StructureGLDrawingArea::initialise() {
	std::cout << "StructureGLDrawingArea::initialise: " << "" << std::endl;

	this->update();

	// Add events.
	//
	add_events(
			Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
					| Gdk::VISIBILITY_NOTIFY_MASK);

}

void StructureGLDrawingArea::setDrawBundle() {
	if (currentBundle == 0) {
			std::cout << "StructureGLDrawingArea::setDrawBundle: " << "WARNING: Bundle Null " << std::endl;
			this->setDrawTest();
		} else {
			this->setDrawMode(DrawMode::DRAW_BUNDLE);
		}
}

void StructureGLDrawingArea::setDrawTest() {
	this->setDrawMode(DrawMode::DRAW_TEST);
}

void StructureGLDrawingArea::setDrawCluster() {
	if (currentCluster == 0) {
		std::cout << "StructureGLDrawingArea::setDrawCluster: " << "WARNING: Cluster Null " << std::endl;
		this->setDrawBundle();
	} else {
		this->setDrawMode(DrawMode::DRAW_CLUSTER);
	}
}

void StructureGLDrawingArea::setDrawNode() {
	if (currentNode == 0) {
			std::cout << "StructureGLDrawingArea::setDrawNode: " << "WARNING: Node Null " << std::endl;
			this->setDrawTest();
		} else {
			this->setDrawMode(DrawMode::DRAW_NODE);
		}
}

void StructureGLDrawingArea::setCurrentBundle(boost::shared_ptr<const Bundle> bun) {
	currentBundle = bun;
}
void StructureGLDrawingArea::setCurrentCluster(boost::shared_ptr<const Cluster> clus) {
}
void StructureGLDrawingArea::setCurrentNode(boost::shared_ptr<const Node> node) {
	currentNode = node;
}

void StructureGLDrawingArea::invalidate() {
	get_window()->invalidate_rect(get_allocation(), false);
}

void StructureGLDrawingArea::setDrawMode(const DrawMode mode) {
	if (drawMode != mode) {
		drawMode = mode;
		this->update();
	}
}

void StructureGLDrawingArea::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void StructureGLDrawingArea::drawBundle() {
	bundleDrawingArea->update();
}

void StructureGLDrawingArea::drawCluster() {
	clusterDrawingArea->update();
}

void StructureGLDrawingArea::drawNode() {
	nodeDrawingArea->update();
}

void StructureGLDrawingArea::drawTest() {
	testDrawingArea->update();
}

}//NAMESPACE

}//NAMESPACE

}//NAMESPACE
