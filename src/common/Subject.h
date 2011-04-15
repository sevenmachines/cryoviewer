/*
 * Subject.h
 *
 *  Created on: 15 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef SUBJECT_H_
#define SUBJECT_H_

#include "Listener.h"
#include <vector>

namespace cryo {

namespace viewer {

namespace common {

template<class T>
class Subject {
public:
	Subject(){}
	virtual ~Subject(){}

	void addListener(boost::shared_ptr<Listener> & listener) {
		listeners.push_back(listener);
	}

	boost::shared_ptr<Listener> removeListener(boost::shared_ptr<Listener> & listener) {
		boost::shared_ptr<Listener> deleted_listener;
		std::vector<boost::shared_ptr<Listener> >::iterator found_listener = listeners.find(listener);
		if (found_listener != 0) {
			deleted_listener = found_listener;
		}
		return deleted_listener;
	}
	void notify(const T & obj){
		// forall in listeners
			 {
				 std::vector<boost::shared_ptr<Listener> >::const_iterator it_listeners = listeners.begin();
				 const  std::vector<boost::shared_ptr<Listener> >::const_iterator it_listeners_end = listeners.end();
				 while ( it_listeners != it_listeners_end){
					 it_listeners->updateListener(obj);
					 ++it_listeners;
				 }
			 }
	}

protected:
	std::vector<boost::shared_ptr<Listener> > listeners;
};

}

}

}

#endif /* SUBJECT_H_ */
