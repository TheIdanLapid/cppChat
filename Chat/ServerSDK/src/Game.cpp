/*
 * Game.cpp
 *
 *  Created on: Sep 25, 2017
 *      Author: eontorch
 */

#include "Game.h"

namespace std {

Game::Game() {
	udp1 = NULL;
	udp2 = NULL;
	tcp2 = NULL;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */

Game::Game(User* user1, User* user2) {
	udp1 = user1->getUdp();
	udp2 = user2->getUdp();
	tcp2 = user2->getTcp();

//	udp1->bind(tcp2->getPort());
	int connect = udp1->connect(udp2->getIP(),tcp2->getPort());
	cout<<udp2->getIP()<<endl;
	cout<<"Port of: "<<user2->getName()<<" "<<tcp2->getPort()<<endl;
	cout<<connect<<endl;
	start();
}

void Game::run() {

	udp1->write("HI");

	char buf[100];
	memset((void*)buf,0,100);


	cout<<"UDPSocketTest: the message : "<<"HI"<<" was sent"<<endl;

	//read the message by the user2
	int cc = udp2->recv(buf, 100);
	cout<<cc<<endl;

	if (cc<0)
	{
		perror("UDPSocketTest: error receiving a message");
	}
	if (cc != 2){
			perror("UDPSocketTest: wrong size");
	}
	//compare the message sent with the message received
	if (strcmp(buf,"HI")!=0){
		perror("UDPSocketTest: wrong message");
	}
	cout<<"UDPSocketTest: the message : "<<buf<<" received"<< endl;


	//test the reply and fromaddr methods
	udp2->reply("checking reply");
	memset((void*)buf,0,100);
	udp1->recv(buf, 100);

	cout<<buf<<endl;
}
