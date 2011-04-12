/*
 * StructureWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef STRUCTUREWINDOW_H_
#define STRUCTUREWINDOW_H_

#include "DisplayWindow.h"
#include "structures/Bundle.h"

namespace cryo {

namespace viewer {

namespace display {

class StructureWindow : public DisplayWindow{
public:
	StructureWindow(const boost::shared_ptr< cryomesh::structures::Bundle > bun);
	virtual ~StructureWindow();

protected:
	virtual void updateData();
		virtual void initialise();
private:
	const boost::shared_ptr< cryomesh::structures::Bundle > bundle;
};

}

}

}

#endif /* STRUCTUREWINDOW_H_ */
