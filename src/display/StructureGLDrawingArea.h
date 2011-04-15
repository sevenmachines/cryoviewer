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

#include <gtkmm/gl/drawingarea.h>

namespace cryo {

namespace viewer {

namespace display {

class StructureGLDrawingArea: public Gtk::GL::DrawingArea {
public:

	/**
	 * Enum representing drawmode
	 */
	enum DrawMode {
		DRAW_BUNDLE, DRAW_CLUSTER
	};

	StructureGLDrawingArea(const boost::shared_ptr<cryomesh::structures::Bundle> bun);
	virtual ~StructureGLDrawingArea();

	virtual void update();
	virtual void setDrawBundle();
	virtual void setDrawCluster(const boost::uuids::uuid & key);

protected:
	virtual void on_realize();
	virtual bool on_configure_event(GdkEventConfigure* event);
	virtual bool on_expose_event(GdkEventExpose* event);

	// Signal handlers:
	virtual bool on_button_press_event(GdkEventButton* event);
	virtual bool on_button_release_event(GdkEventButton* event);
	virtual bool on_motion_notify_event(GdkEventMotion* event);

	virtual void initialise();

	virtual void setDrawMode(const DrawMode mode);

	virtual void clear();
	virtual void drawBundle();
	virtual void drawCluster() ;

	virtual void drawClusters();
	virtual void drawFibres();
	virtual void drawConnections();
	virtual void drawNodes();

	virtual void drawMappedPoints(const std::map<boost::uuids::uuid, cryomesh::spacial::Point> & map );
	virtual void drawMappedJoins(const std::map<boost::uuids::uuid,  std::pair<boost::uuids::uuid, boost::uuids::uuid> > & map,
			const std::map<boost::uuids::uuid, cryomesh::spacial::Point> & pointmap );
	virtual void drawSphere(const cryomesh::spacial::Point & centre, double radius=1  );
	virtual void drawLine(const cryomesh::spacial::Point & start,  const cryomesh::spacial::Point & end, double thickness=1  );

	virtual void invalidate();
	virtual boost::uuids::uuid doPick() const;

	const boost::shared_ptr<cryomesh::structures::Bundle> bundle;
	boost::shared_ptr<const cryomesh::structures::Cluster> currentCluster;

	DrawMode drawMode;
	boost::uuids::uuid drawUUID;

	/**
	 * Map of cluster uuid to point in space
	 */
	std::map<boost::uuids::uuid, cryomesh::spacial::Point> clusterSpacialMap;

	/**
	 * Map of fibre uuid to a pair of input and output cluster uuids
	 */
	std::map<boost::uuids::uuid,  std::pair<boost::uuids::uuid, boost::uuids::uuid> > fibreSpacialMap;

	/**
	 * Map of node uuid to point in space
	 */
	std::map<boost::uuids::uuid, cryomesh::spacial::Point> nodeSpacialMap;

	/**
	 * Map of connection uuid to a pair of input and output node uuids
	 */
	std::map<boost::uuids::uuid,  std::pair<boost::uuids::uuid, boost::uuids::uuid> > connectionSpacialMap;

};

}

}

}

#endif /* STRUCTUREGLDRAWINGAREA_H_ */
