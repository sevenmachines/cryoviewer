/*
 * StructureGLDrawingArea.cpp
 *
 *  Created on: 14 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "StructureGLDrawingArea.h"

#include <GL/gl.h>

#include <GL/glu.h>
namespace cryo {

namespace viewer {

namespace display {

using namespace cryomesh::components;
StructureGLDrawingArea::StructureGLDrawingArea(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
	bundle(bun), drawMode(DRAW_BUNDLE) {
	// Configure OpenGL-capable visual.
	Glib::RefPtr<Gdk::GL::Config> glconfig;
	// Try double-buffered visual
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
	if (!glconfig) {
		std::cerr << "*** Cannot find the double-buffered visual.\n" << "*** Trying single-buffered visual.\n";

		// Try single-buffered visual
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH);
		if (!glconfig) {
			std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
			std::exit(1);
		}
	}
	this->set_gl_capability(glconfig);
	this->initialise();

	this->drawBundle();

	this->show();
}

StructureGLDrawingArea::~StructureGLDrawingArea() {
	// TODO Auto-generated destructor stub
}

void StructureGLDrawingArea::update() {
	if (drawMode == DrawMode::DRAW_BUNDLE) {
		if (bundle != 0) {
			// get clusters representation

			// get fibres representation

		} else {
			std::cout << "StructureGLDrawingArea::update: " << "ERROR: Not updating null bundle." << std::endl;
		}

	} else if (drawMode == DrawMode::DRAW_CLUSTER) {
		if (currentCluster != 0) {
			nodeSpacialMap.clear();
			connectionSpacialMap.clear();
			// get nodes representation
			const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & all_nodes = currentCluster->getNodes();
			// forall in all_nodes
			{
				std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes = all_nodes.begin();
				const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_all_nodes_end =
						all_nodes.end();
				while (it_all_nodes != it_all_nodes_end) {
					nodeSpacialMap[it_all_nodes->first] = it_all_nodes->second->getPosition();
					++it_all_nodes;
				}
			}
			// get connections representation
			const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> > & all_connections =
					currentCluster->getConnections();
			// forall in all_connections
			{
				std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator it_all_connections =
						all_connections.begin();
				const std::map<boost::uuids::uuid, boost::shared_ptr<Connection> >::const_iterator
						it_all_connections_end = all_connections.end();
				while (it_all_connections != it_all_connections_end) {
					boost::uuids::uuid inid = it_all_connections->second->getConnector().getInputs().begin()->first;
					boost::uuids::uuid outid = it_all_connections->second->getConnector().getOutputs().begin()->first;
					std::pair<boost::uuids::uuid, boost::uuids::uuid> conpair(inid, outid);
					connectionSpacialMap[it_all_connections->first] = conpair;
					++it_all_connections;
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
	this->set_size_request(400, 200);

	// Add events.
	//
	add_events(
			Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
					| Gdk::VISIBILITY_NOTIFY_MASK);

}

void StructureGLDrawingArea::on_realize() {
	std::cout << "StructureGLDrawingArea::on_realize: " << "" << std::endl;
	Gtk::GL::DrawingArea::on_realize();
	// Get GL::Window.
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	// Do opengl in here
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return;

	 GLUquadricObj* qobj = gluNewQuadric();
	 gluQuadricDrawStyle(qobj, GLU_FILL);
	 glNewList(1, GL_COMPILE);
	 gluSphere(qobj, 1.0, 20, 20);
	 glEndList();

	 static GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	 static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	 glEnable(GL_LIGHTING);
	 glEnable(GL_LIGHT0);
	 glEnable(GL_DEPTH_TEST);

	 glClearColor(1.0, 1.0, 1.0, 1.0);
	 glClearDepth(1.0);

	 glViewport(0, 0, get_width(), get_height());

	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 gluPerspective(40.0, 1.0, 1.0, 10.0);

	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	 glTranslatef(0.0, 0.0, -100.0);
	// End of opengl
	glwindow->gl_end();
}
bool StructureGLDrawingArea::on_configure_event(GdkEventConfigure* event) {
	std::cout << "StructureGLDrawingArea::on_configure_event: " << "" << std::endl;
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	//glViewport(0,0,this->get_width(), this->get_height());
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	if (!glwindow->gl_begin(get_gl_context()))
		return false;

	glViewport(0, 0, get_width(), get_height());

	glwindow->gl_end();

	return true;
}

bool StructureGLDrawingArea::on_expose_event(GdkEventExpose* event) {
	std::cout << "StructureGLDrawingArea::on_expose_event: " << "" << std::endl;
	// Get GL::Window.
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return false;

	this->clear();
	if (drawMode == DrawMode::DRAW_BUNDLE) {
		this->drawBundle();
	} else if (drawMode == DrawMode::DRAW_CLUSTER) {
		this->drawCluster();
	} else {
		std::cout << "StructureGLDrawingArea::on_expose_event: " << "WARNING: Unknown draw mode" << std::endl;
	}
	// Swap buffers.
	if (glwindow->is_double_buffered())
		glwindow->swap_buffers();
	else
		glFlush();

	glwindow->gl_end();
	// *** OpenGL END ***

	return true;
}

void StructureGLDrawingArea::setDrawBundle() {
	this->setDrawMode(DrawMode::DRAW_BUNDLE);
}
void StructureGLDrawingArea::setDrawCluster(const boost::uuids::uuid & key) {
	currentCluster = bundle->getClusters().getObjectByKey(key);
	if (currentCluster == 0) {
		std::cout << "StructureGLDrawingArea::setDrawMode: " << "WARNING: Cluster not found, " << "'" << key << "'"
				<< std::endl;
		this->setDrawBundle();
	} else {
		this->setDrawMode(DrawMode::DRAW_CLUSTER);
	}
}

void StructureGLDrawingArea::invalidate() {
	get_window()->invalidate_rect(get_allocation(), false);
}

bool StructureGLDrawingArea::on_button_press_event(GdkEventButton* event) {
	std::cout << "StructureGLDrawingArea::on_button_press_event: " << "" << std::endl;
	// don't block
	return false;
}

bool StructureGLDrawingArea::on_button_release_event(GdkEventButton* event) {
	std::cout << "StructureGLDrawingArea::on_button_release_event: " << "" << std::endl;
	return false;
}

bool StructureGLDrawingArea::on_motion_notify_event(GdkEventMotion* event) {
	float w = this->get_width();
	float h = this->get_height();
	float x = event->x;
	float y = event->y;
	bool redraw = false;
	std::cout << "StructureGLDrawingArea::on_motion_notify_event: " << "" << std::endl;

	if (redraw == true) {
		this->invalidate();
	}
	// don't block
	return false;
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
	std::cout << "StructureGLDrawingArea::drawBundle: " << bundle->getClusters().getSize() << std::endl;
	this->drawClusters();
	this->drawFibres();
}

void StructureGLDrawingArea::drawCluster() {
	std::cout << "StructureGLDrawingArea::drawCluster: " << currentCluster->getNodeMap().getSize() << std::endl;
	this->drawNodes();
	this->drawConnections();
}

void StructureGLDrawingArea::drawClusters() {
	std::cout << "StructureGLDrawingArea::drawClusters: " << clusterSpacialMap.size() << std::endl;
	this->drawMappedPoints(clusterSpacialMap);
}

void StructureGLDrawingArea::drawFibres() {
	std::cout << "StructureGLDrawingArea::drawFibres: " << fibreSpacialMap.size() << std::endl;
	this->drawMappedJoins(fibreSpacialMap, clusterSpacialMap);
}

void StructureGLDrawingArea::drawConnections() {
	std::cout << "StructureGLDrawingArea::drawConnections: " << connectionSpacialMap.size() << std::endl;
	this->drawMappedJoins(connectionSpacialMap, nodeSpacialMap);
}

void StructureGLDrawingArea::drawNodes() {
	std::cout << "StructureGLDrawingArea::drawNodes: " << nodeSpacialMap.size() << std::endl;
	this->drawMappedPoints(nodeSpacialMap);
}

void StructureGLDrawingArea::drawMappedPoints(const std::map<boost::uuids::uuid, cryomesh::spacial::Point> & map) {
	// forall in map
	{
		std::map<boost::uuids::uuid, cryomesh::spacial::Point>::const_iterator it_map = map.begin();
		const std::map<boost::uuids::uuid, cryomesh::spacial::Point>::const_iterator it_map_end = map.end();
		while (it_map != it_map_end) {
			this->drawSphere(it_map->second);
			++it_map;
		}
	}
}

void StructureGLDrawingArea::drawMappedJoins(
		const std::map<boost::uuids::uuid, std::pair<boost::uuids::uuid, boost::uuids::uuid> > & map,
		const std::map<boost::uuids::uuid, cryomesh::spacial::Point> & pointmap) {

	// forall in map
	{
		std::map<boost::uuids::uuid, std::pair<boost::uuids::uuid, boost::uuids::uuid> >::const_iterator it_map =
				map.begin();
		const std::map<boost::uuids::uuid, std::pair<boost::uuids::uuid, boost::uuids::uuid> >::const_iterator
				it_map_end = map.end();
		while (it_map != it_map_end) {
			boost::uuids::uuid startid = it_map->second.first;
			boost::uuids::uuid endid = it_map->second.second;

			cryomesh::spacial::Point start_point;
			cryomesh::spacial::Point end_point;

			std::map<boost::uuids::uuid, cryomesh::spacial::Point>::const_iterator it_start_found = pointmap.find(
					startid);
			std::map<boost::uuids::uuid, cryomesh::spacial::Point>::const_iterator it_end_found = pointmap.find(endid);

			if (it_start_found != pointmap.end()) {
				start_point = it_start_found->second;
				if (it_end_found != pointmap.end()) {
					end_point = it_end_found->second;
					this->drawLine(start_point, end_point);
				} else {
					std::cout << "StructureGLDrawingArea::drawMappedJoins: "
							<< "WARNING: End point for line not found, " << "'" << it_map->first << "'" << std::endl;
				}
			} else {
				std::cout << "StructureGLDrawingArea::drawMappedJoins: " << "WARNING: Start point for line not found, "
						<< "'" << it_map->first << "'" << std::endl;
			}
			++it_map;
		}
	}

}
void StructureGLDrawingArea::drawSphere(const cryomesh::spacial::Point & centre, double radius) {
	//std::cout << "StructureGLDrawingArea::drawSphere: " << "(" << centre.getX() << ", " << centre.getY() << ", "
	//		<< centre.getZ() << ")" << std::endl;
	const int SLICES = 10;
	const int STACKS = 10;
	const double RADIUS = 1.0;

	glPushMatrix();
	GLUquadric * quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	glColor3f(0.5, 0.5, 0.5);
	glTranslated(centre.getX(), centre.getY(), centre.getZ());
	gluSphere(quad, RADIUS, SLICES, STACKS);
	gluDeleteQuadric(quad);
	glPopMatrix();
}
void StructureGLDrawingArea::drawLine(const cryomesh::spacial::Point & start, const cryomesh::spacial::Point & end,
		double thickness) {
//	std::cout << "StructureGLDrawingArea::drawLine: " << "TODO" << std::endl;
}

boost::uuids::uuid StructureGLDrawingArea::doPick() const {
	boost::uuids::uuid foundobj;
	std::cout << "StructureGLDrawingArea::doPick: " << "" << std::endl;
	return foundobj;
}
}//NAMESPACE

}//NAMESPACE

}//NAMESPACE
