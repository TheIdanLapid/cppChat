/*
 * Socket.h
 *
 *  Created on: Jun 14, 2017
 *      Author: user
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "MThread.h"
#include "FileBase.h"
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
using namespace std;

class Socket : public FileBase
{
public:
	//when the tcp is tcp client - local is client & remote is server
	//when the tcp is tcp server - local is server & remote is client
	struct sockaddr_in local, remote;
	//the fd of the current socket
	int socket_fd;
	Socket();
	Socket(int type);
	int bind(int port);
	char * getIP() const;
	virtual int connect(string ip, int port);
	virtual int read(char* buff, int size);
	int write(const string& msg);
	virtual void close();
	virtual ~Socket();
};

#endif /* SOCKET_H_ */
