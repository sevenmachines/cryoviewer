/*
 * ActiveWindow.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ACTIVEWINDOW_H_
#define ACTIVEWINDOW_H_

#include "DisplayWindow.h"
namespace cryo {

namespace viewer {

namespace display {

class ActiveWindow: public DisplayWindow {
public:
	ActiveWindow();
	virtual ~ActiveWindow();
};

}

}

}

#endif /* ACTIVEWINDOW_H_ */
