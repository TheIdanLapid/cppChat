/*
 * Dispatcher.h
 *
 *  Created on: Sep 24, 2017
 *      Author: user
 */

#include "MThread.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "MTCPListener.h"
#include "Guard.h"
#include <string>
#include <strings.h>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FileManager.h"
#include "UserManager.h"
#include "Protocol.h"
#include "netinet/in.h"
#include <vector>
#include <map>

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

using namespace std;

class DispatcherHandler{
public:
	virtual void closeGame(User* user) = 0;
	virtual int openGameSession(User* user1, User* user2) = 0;
	virtual ~DispatcherHandler();
};

class Dispatcher: public MThread {
	FileManager* fm;
	UserManager* um;
	typedef vector<TCPSocket*> tSockets;
	map<TCPSocket*, User*>* tcpMap; //Maps a user to it's tcpSocket
	MTCPListener* waitingPeers;
	MTCPListener* peers;
	DispatcherHandler* handler;
	bool peersOn,closed;
	pthread_mutex_t mutex;
	bool peeread;

	User* getUserFromTCPMap(TCPSocket* sock);
	User* getUserFromTCPMapByPort(int port);
public:
	Dispatcher(DispatcherHandler* handler);
	void addPeer(TCPSocket* peer);
	void addLoggedPeer(TCPSocket* sock);

//	bool removePeer(TCPSocket* peer);
	int sendCommandOnly(TCPSocket* peer, int command);
	int readCommand(TCPSocket* peer);
	int sendCommand(TCPSocket* peer, int command, const char* buff);
//	int getPeerIndex(string ip,int port);
	bool removePeerFromVector(TCPSocket* peer);
	void updateScore(User* user, int score);
	virtual void run();
//	void close();
	void printLoggedUsers(TCPSocket* peer);
	void disconnectUser(TCPSocket* peer);
	virtual ~Dispatcher();
};

#endif /* DISPATCHER_H_ */
