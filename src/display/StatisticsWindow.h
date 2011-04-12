/*
 * StatisticsWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef STATISTICSWINDOW_H_
#define STATISTICSWINDOW_H_
#include "DisplayWindow.h"
#include "structures/Bundle.h"

namespace cryo {

namespace viewer {

namespace display {

class StatisticsWindow  : public DisplayWindow{
public:
	StatisticsWindow(const boost::shared_ptr< cryomesh::structures::Bundle > bun);
	virtual ~StatisticsWindow();

protected:
	virtual void updateData();
		virtual void initialise();
private:
	const boost::shared_ptr< cryomesh::structures::Bundle > bundle;
};

}

}

}

#endif /* STATISTICSWINDOW_H_ */
