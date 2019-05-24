/*
 * UDPListener.h
 *
 *  Created on: Oct 21, 2017
 *      Author: user
 */

#ifndef UDPLISTENER_H_
#define UDPLISTENER_H_

#include <string>
#include <strings.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include "UDPSocket.h"
#include "MThread.h"


namespace std {

class UDPListener : public MThread {
	string lastMSG;
	UDPSocket* udp;
	pthread_mutex_t mutex;
public:
	UDPListener(UDPSocket* udp);
	void setMSG(string msg);
	string getMSG();
	void run();
	virtual ~UDPListener();
};

} /* namespace std */

#endif /* UDPLISTENER_H_ */
