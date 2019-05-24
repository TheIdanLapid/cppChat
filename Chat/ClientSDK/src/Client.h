/*
 * Client.h
 *
 *  Created on: Oct 4, 2017
 *      Author: user
 */

#ifndef CLIENT_H_
#define CLIENT_H_
#include <string>
#include <strings.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "ClientHandler.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "MThread.h"
#include "Protocol.h"
#include "Guard.h"
#include "UDPListener.h"


namespace std {

class Client : public MThread {
	UDPListener* udpl;
	ClientHandler* handler;
	bool connected;
	bool onGame;
	bool closed;
	bool logged;
	string opponent;
	string userList;
	pthread_mutex_t mutex;
	TCPSocket* socket;
	UDPSocket* udp;

	//opponent variables
	string portStrOpp;
	int oppPort;
	string oppIp;
	UDPSocket* opp;

	int number;
	int guess;
	int oppBuf;

public:
	Client(ClientHandler* handler);
	void run();

	//inner functions
	int startTheGame();
	//gets
	string getOpponentName();
	string getUserList();
	int getPort();
	TCPSocket* getSocket();
	UDPSocket* getUDPSocket();
	ClientHandler* getHandler();
	bool isOnGame();
	bool isConnected();
	bool isClosed();
	bool isLogged();
	string getOppIp();
	int getOppPort();

	//sets
	void setGuess(int guess);
	void setNumber(int number);
	void setLogged(bool val);
	void setConnected(bool val);
	void setClosed(bool val);
	void setOnGame(bool val);
	void setOppIp(string ip);
	void setOppPort(int port);

	//to server
	int sendCommandToServer(TCPSocket* socket, int com);
	int sendCommandToServer(TCPSocket* socket, int com, string rest);
	bool connect(string ip);
	void registerUser(string user,string pass);
	void login(string user ,string pass);
	bool openGame(string name);
	bool openGameRandom();
	void acceptGame();
	void declineGame();
	void displayUsers();
//	void readUDP();
	int readcommand(string& data);
	void sendMessageUDP(string msg);
	void closeGame();
	void disconnect();
	virtual ~Client();
};

} /* namespace std */

#endif /* CLIENT_H_ */
