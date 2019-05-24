/*
 *  UDPSocket.h
 *
 *  Created on: Apr 23, 2017
 *      Author: Idan & Shdema
 */

#ifndef SRC_UDPSOCKET_H_
#define SRC_UDPSOCKET_H_
#include "Socket.h"
using namespace std;

class UDPSocket : public Socket{

public:
	virtual ~UDPSocket();

	UDPSocket(int port = 9999);

	int recv(char* buffer, int length);
	
	int sendTo(string msg, string ip, int port);
	
	int reply(string msg);
	
//	virtual void close();

	string fromAddr();

};


#endif
