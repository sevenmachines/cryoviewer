/*
 * NodeActivityDrawingArea.h
 *
 *  Created on: 01-Jun-2009
 *      Author: niall
 */

#ifndef FIBREACTIVITYDRAWINGAREA_H_
#define FIBREACTIVITYDRAWINGAREA_H_

/*
 *
 */
#include "ActivityDrawingArea.h"
#include <boost/shared_ptr.hpp>
#include <components/Node.h>

namespace cryo {

namespace viewer {

namespace display {

class NodeActivityDrawingArea: public ActivityDrawingArea {
public:
	NodeActivityDrawingArea(const boost::shared_ptr<cryomesh::components::Node> & nd);
	virtual ~NodeActivityDrawingArea();

	void update();
protected:
	virtual void drawText();
	virtual void drawAxis();
	void setAsPrimaryInput();
	void setAsPrimaryOutput();

private:
	const boost::shared_ptr<cryomesh::components::Node> node;
	static const int MAX_FUTURE_TICKS;
	bool isPrimaryInput;
	bool isPrimaryOutput;
};

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE

#endif /* FIBREACTIVITYDRAWINGAREA_H_ */
