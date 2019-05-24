/*
 * User.h
 *
 *  Created on: Sep 4, 2017
 *      Author: idan
 */

#ifndef USER_H_
#define USER_H_
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "MThread.h"
#include <stdlib.h>
#include <sys/socket.h>
#include "Protocol.h"

namespace std {

class User : MThread {
	string name;
	string pass;
	int score;
	TCPSocket* tcp;
	UDPSocket* udp;
	bool connected;
	bool onGame;
	bool loggedIn;
public:
	User();
	User(User* user);
	User(string name, string pass);
	User(string name, string pass, int score);
	bool login();
	bool openMatch(string name); //Open a match with a chosen player
	bool openMatch(); //Open a match with a random player
	void run();
	virtual ~User();
	bool isConnected() const;
	void setConnected(bool connected);
	bool isLoggedIn() const;
	void setLoggedIn(bool loggedIn);
	const string& getName();
	void setName(const string& name);
	bool isOnGame() const;
	void setOnGame(bool onGame);
	const string& getPass() const;
	void setPass(const string& pass);
	int getScore() const;
	void setScore(int score);
	TCPSocket* getTcp();
	void setTcp(TCPSocket* tcp);
	UDPSocket* getUdp();
	void setUdp(UDPSocket* udp);
};

}

#endif /* USER_H_ */
