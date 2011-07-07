/*
 * Listener.h
 *
 *  Created on: 15 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef LISTENER_H_
#define LISTENER_H_

namespace cryo {

namespace viewer {

namespace common {

template <class T>
class Listener {
public:
	Listener(){}
	virtual ~Listener(){}
	void updateListener(const T & obj)=0;
};

}

}

}

#endif /* LISTENER_H_ */
