/*
 * Protocol.h
 *
 *  Created on: Sep 4, 2017
 *      Author: idan
 */
#include <map>
#include <string>


#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/**
 * Game Protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
using namespace std;

#define SERVER_PORT 3346
#define IP "127.0.0.1"

typedef map<string,pair<string,int> > uMap;

#define OUTPUT					0	///just a msg from the server to print
#define REGISTER				1	//Followed by user:pass
#define LOGIN					2	//Followed by user:pass
#define SHOW_USERS				3
#define OPEN_MATCH_WITH_USER 	4	//Followed by user:ip:port
#define OPEN_MATCH_RANDOM		5	//Followed by ip:port
#define ACCEPT_MATCH			6
#define DECLINE_MATCH			7
#define EXIT_MATCH				8
#define EXIT_SERVER				9
#define USERLIST				10
#define OPP_SCORE				11

#define PATH "src/users.txt"
#define WRONG_USER				-1
#define WRONG_PASS				-2
#define INVITATION				-3

#endif /* PROTOCOL_H_ */
