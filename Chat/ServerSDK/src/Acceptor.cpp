/*
 * Acceptor.cpp
 *
 *  Created on: Sep 24, 2017
 *      Author: user
 */

#include "Acceptor.h"

Acceptor::Acceptor(AcceptorHandler* handler) {
	this->handler = handler;
	socket = NULL;
	exitFromServer = false;
}

void Acceptor::run() {
	cout<<"Server: Server is running..."<<endl;
	socket = new TCPSocket(SERVER_PORT);
	TCPSocket* peer = NULL;
	while (!exitFromServer) {
		peer = socket->listenAndAccept();
		if (peer == NULL) {
			cout<<"Server: can't connect the peer"<<endl;
		}
		else {
			cout<<"Server: "<<peer->getPort()<<" REMOTE PEER PORT"<<endl;
			handler->handlePeer(peer);
		}
	}
}

Acceptor::~Acceptor() {}

