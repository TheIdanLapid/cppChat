/*
 * MTCPListener.h
 *
 */

#ifndef MTCPLISTENER_H_
#define MTCPLISTENER_H_

//#include <sys/socket.h>
#include "TCPSocket.h"
#include <vector>

using namespace std;

class MTCPListener {
	typedef vector<TCPSocket*> tSockets;
public:
	vector<TCPSocket*> sockets;
	/**
	 * add socket to listen to
	 */
	void add(TCPSocket* socket);
	int getSocketIndex(string ip, int port);

	void addAll(vector<TCPSocket*> sockets);
	TCPSocket* listen(int timeout);

};

#endif /* MTCPLISTENER_H_ */
