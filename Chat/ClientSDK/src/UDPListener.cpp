/*
 * UDPListener.cpp
 *
 *  Created on: Oct 21, 2017
 *      Author: user
 */

#include "UDPListener.h"

namespace std {

UDPListener::UDPListener(UDPSocket* sock) {
	lastMSG = "";
	udp = sock;
	start();
}

void UDPListener::setMSG(string msg) {
	lastMSG = msg;
}

void UDPListener::run() {
	char buf[100];
	memset((void*)buf,0,100);
	int rc = udp->recv(buf, 100);
	cout<<rc<<endl;
	if (rc != -1)
	{
		stringstream strs;
		strs << buf;
		string message = strs.str();
		cout<<buf<<endl;
		setMSG(message);
	}
	else{
		cout<<"Udp listener running but didnt get a message"<<endl;
	}
}

string UDPListener::getMSG() {
	return lastMSG;
}

UDPListener::~UDPListener() {
}

} /* namespace std */
