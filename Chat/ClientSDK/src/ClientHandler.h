/*
 * ClientHandler.h
 *
 *  Created on: Oct 9, 2017
 *      Author: user
 */

#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_
#include <iostream>
#include <unistd.h>
#include <stdio.h>
using namespace std;
class ClientHandler {
	string lastMSG;
	bool invited;
public:
		ClientHandler();
		string getMSG();
		void setMSG(string msg);
		bool isInvited();
		void setInvited(bool inv);
		virtual ~ClientHandler();
};


#endif /* CLIENTHANDLER_H_ */
