/*
 * TCPSocket.h
 *
 *  Created on: May 8, 2017
 *      Author: user
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_
#include "Socket.h"
using namespace std;

class TCPSocket : public Socket{

public:
	TCPSocket (int port); // constructor to create a TCP server socket.
	TCPSocket (string peerIp, int port); // constructor to creates TCP client socket
										// and connect it to the remote peer in the given ip and port.
	TCPSocket (int socket_fd, struct sockaddr_in serv_name,	struct sockaddr_in cli_name);
	TCPSocket* listenAndAccept (); // perform listen and accept on server socket,
								  //this function returns a new TCPSocket object to communicate over the incoming
								 //session.
	int recv (char* buffer, int length); // read from socket into the given buffer up to
										//the buffer given length, return the number of bytes read.
	int send (const string& msg); // send the given buffer to the socket.
	int write(const char* msg,int size);
	int write(const string& msg);

	int getPort();
	string getIP();

//	void close (); // close the socket and free all resources.
	string fromAddr (); // return the address of the connected peer.
	virtual ~TCPSocket();

	int getSocketFd() const;
};

#endif /* TCPSOCKET_H_ */
