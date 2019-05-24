/*
 * Acceptor.h
 *
 *  Created on: Sep 24, 2017
 *      Author: user
 */
#include "TCPSocket.h"
#include "MThread.h"
#include "Protocol.h"


#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

class AcceptorHandler {
public:
	virtual void handlePeer(TCPSocket* peer) = 0;
	virtual ~AcceptorHandler();
};

class Acceptor : public MThread {
	TCPSocket* socket;
	bool exitFromServer;
	AcceptorHandler* handler;
public:
	Acceptor(AcceptorHandler* handler);
	void run();
	virtual ~Acceptor();
};

#endif /* ACCEPTOR_H_ */
