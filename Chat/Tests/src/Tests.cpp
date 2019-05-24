
#include <iostream>
using namespace std;
#include "FileManager.h"
#include "User.h"
#include "Protocol.h"
#include <vector>
#include "UserManager.h"
#include "netinet/in.h"
#include "ServerSDK.h"
#include "Dispatcher.h"
#include "Acceptor.h"

using namespace std;

//Move me to CLIENT
//int sendCommand(TCPSocket* socket, int com, string rest) {
//	int cmdNet = htonl(com);
//	int res = socket->write((char*)&cmdNet,4); //the command int from the protocol
//	if (res<4)
//		return -1;
//	int len = rest.length();
//	int lenNet = htonl(len);
//	res = socket->write((char*)&lenNet,4); //the length of the message
//	if (res<4)
//		return -1;
//	res = socket->write(rest.c_str(),len); //the message
//	if (res<4)
//		return -1;
//	return 1;
//}
//
//int sendCommand(TCPSocket* socket, int com) {
//	int cmdNet = htonl(com);
//	int res = socket->write((char*)&cmdNet,4); //the command int from the protocol
//	if (res<4)
//		return -1;
//	return 1;
//}

void printMap(FileManager* fm) {
	cout<<"printinggGgggg"<<endl;
	uMap* map = fm->getUsersMap(); //uMap is defined in Protocol.h as: map<string,pair<string,int> >
	uMap::iterator pos;
	for (pos = (*map).begin(); pos != (*map).end(); ++pos) {
		cout << "key: " << pos->first << " values: pass: " << pos->second.first << " score: " <<pos->second.second << endl;
	}
}

int main() {

//	FileManager* fm = new FileManager();
//	fm->clearFile();
//
//	User user;
//	UserManager* um = new UserManager(fm);
//	user.setName("Idan");
//	user.setPass("sisma1");
//	user.setScore(2);
//	fm->writeUser(user);
//	cout<<"Finished writing "<<user.getName()<<endl;
//
//	User shdema;
//	shdema.setName("Shdema");
//	shdema.setPass("sisma2");
//	shdema.setScore(56);
//	fm->writeUser(shdema);
//	cout<<"Finished writing "<<shdema.getName()<<endl;

//	fm->readUsers();
//	printMap(fm);
//	uMap* map = fm->getUsersMap(); //uMap is defined in Protocol.h as: map<string,pair<string,int> >
//	uMap::iterator pos;
//	for (pos = (*map).begin(); pos != (*map).end(); ++pos) {
//		cout << "key: " << pos->first << " values: pass: " << pos->second.first << " score: " <<pos->second.second << endl;
//	}




	ServerSDK* server = new ServerSDK();
	server->start();
//	sleep(2);
//	TCPSocket* socket = new TCPSocket(IP,SERVER_PORT);
//	TCPSocket* sock1 = new TCPSocket(IP,SERVER_PORT);
//	server->handlePeer(socket);
//	string name = "Rotem";
//	string pass = "Lapid";
//	server->dispatcher->um->registerUser(name,pass);
//
//	fm->setScore(shdema,7);
//
//	User user4;
//	user4.setName("Oren");
//	user4.setPass("sisma6");
//	user4.setScore(9);
//	fm->writeUser(user4);
//	cout<<"Finished writing "<<user4.getName()<<endl;
//
//	cout<<um->loginUser("Rotem","Lapid")<<endl;
//	cout<<um->loginUser("Rotem","Lapids")<<endl;
//	cout<<um->loginUser("Rostem","Lapid")<<endl;

//	sleep(2);
//
//	int success;
//
//	/*
//	 * REGISTER
//	 */
//
//
//	success = sendCommand(socket, REGISTER, "idan:1234");
//
//	if (success) {
//		cout<<"Registered!"<<endl;
//	}

	/*
	 * LOGIN
	 */

//	success = sendCommand(socket, LOGIN, "idan:1234");
//
//	if (success) {
//		cout<<"Logged in!"<<endl;
//	}
//
//	sleep(2);
//
//	/*
//	 * SHOW USERS
//	 */
//
//	success = sendCommand(socket, SHOW_USERS);

	/*
	 * REGISTER
	 */

//
//	success = sendCommand(sock1, REGISTER, "shdema:acdsx");
//
//	if (success) {
//		cout<<"Registered!"<<endl;
//	}

	/*
	 * LOGIN
	 */

//	success = sendCommand(sock1, LOGIN, "shdema:acdsx");
//
//	if (success) {
//		cout<<"Logged in!"<<endl;
//	}
//
//	sleep(2);

	/*
	 * SHOW USERS
	 */

//	success = sendCommand(socket, SHOW_USERS);
//
//	success = sendCommand(sock1, OPEN_MATCH_WITH_USER, "idan");
//
//	sleep(3);
//
//	success = sendCommand(socket, DECLINE_MATCH);
//
//	success = sendCommand(sock1, OPEN_MATCH_RANDOM);
//
//	sleep(3);
//
//	success = sendCommand(socket, ACCEPT_MATCH);


	while (true){}
	cout<<"End of TEST"<<endl;


	return 0;
}
