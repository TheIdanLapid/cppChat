/*
 * Dispatcher.cpp
 *
 *  Created on: Sep 24, 2017
 *      Author: user
 */

#include "Dispatcher.h"
using namespace std;

Dispatcher::Dispatcher(DispatcherHandler* handler) {
	this->handler = handler;
	this->peersOn = false;
	this->closed = false;
	this->peers = new MTCPListener();
	this->waitingPeers = new MTCPListener();
	mutex = PTHREAD_MUTEX_INITIALIZER;
	peeread = false;
	tcpMap = new map<TCPSocket*, User*>();
	fm = new FileManager();
	um = new UserManager(fm);
}

//bool Dispatcher::removePeer(TCPSocket* peer){
//	if (peer==NULL)
//	{
//		cout<<"Dispatcher: PEER IS NULL!"<<endl;
//		return false;
//	}
//	int i = 0;
//	tSockets::iterator iter = peers->sockets.begin();
//	for(;iter!= peers->sockets.end();iter++)
//	{
//		if (*iter!=NULL)
//		{
//			TCPSocket* curr = *iter;
////			cout<<curr<<"Dispatcher: is the current socket with port: "<<curr->getPort()<<endl;
//			if(curr->getIP()==peer->getIP() && curr->getPort()==peer->getPort()){
////				cout<<"Dispatcher: Peer index to delete: "<<i<<endl;
//				cout<<"Dispatcher: Peer to delete: "<<peers->sockets[i]->getPort()<<endl;
//				Guard guard(&mutex);
//				peers->sockets.erase(peers->sockets.begin()+i);
//				if(peers->sockets.size()==0){
//					peersOn = false;
//				}
//				return true;
//			}
//			i++;
//		}
//		else
//		{
//			cout<<"Dispatcher: iter null"<<endl;
//		}
//	}
//	return false;
//
//}

void Dispatcher::addPeer(TCPSocket* sock) {
//	waitForThread();
	Guard guard(&mutex);
	User* user = new User();
	user->setTcp(sock);
	//um->getUnknownUsers().push_back(user); //Add to waiting users vector
	waitingPeers->add(sock); //Add to waiting sockets vector
//	tcpMap->insert(make_pair(sock,user)); //Add to map
	cout << "Dispatcher: Added peer to waitingPeers list with port: "
			<< sock->getPort() << endl;
}
void Dispatcher::addLoggedPeer(TCPSocket* peer) {
	if (peer == NULL) {
		cout << "Dispatcher: PEER IS NULL!" << endl;
	}
	else {
		int i = 0;
		tSockets::iterator iter = waitingPeers->sockets.begin();
		for (; iter != waitingPeers->sockets.end(); iter++) {
			if (*iter != NULL) {
				TCPSocket* curr = *iter;
				if (curr->getIP() == peer->getIP() && curr->getPort() == peer->getPort()) {
					cout << "Dispatcher: Peer to delete: "
							<< waitingPeers->sockets[i]->getPort() << endl;
					Guard guard(&mutex);
					waitingPeers->sockets.erase(waitingPeers->sockets.begin() + i);
					peers->sockets.push_back(peer);
					if (waitingPeers->sockets.size() == 0) {
						peersOn = false;
					}
				}
				i++;
			}
			else {
				cout << "Dispatcher: iter null" << endl;
			}
		}
	}
}

int Dispatcher::sendCommandOnly(TCPSocket* peer, int command) {
	int cmdNet = htonl(command);
	int res = peer->write((char*) &cmdNet, 4); //the command int from the protocol
	if (res < 4)
		return -1;
	return 1;
}

void Dispatcher::run() {
	time_t t;
	map<TCPSocket*, User*>::iterator iter;
	srand((unsigned) time(&t));
	int r;
	while (!closed) {
		r = rand();
		//		cout<<"Dispatcher: size of peers: "<<peers->sockets.size()<<endl;
			MTCPListener allPeers;

			allPeers.addAll(peers->sockets);
			allPeers.addAll(waitingPeers->sockets);

			peers->sockets.clear();
			waitingPeers->sockets.clear();

			//unify both vectors to mtcp every time

			peers->sockets = allPeers.sockets;
		std::vector<TCPSocket*> path;
		int index = 0;
		for (std::vector<TCPSocket*>::const_iterator i = path.begin();
				i != path.end(); ++i) {
			cout << "Socket #" << index << endl;
			std::cout << *i << ' ';
			index++;
		}
		//		if (peersOn&& !peeread){
		TCPSocket* peer = allPeers.listen(4);

		if (!peer) { //timeout without any peer ready
//				cout<<"Dispatcher: there is no ready peer"<<endl;
			continue;
		} else {
			//cout<<"Dispatcher: peer: "<<peer->getPort()<<" is now connected!"<<endl;
			int command = readCommand(peer);
//			cout << "Received command: " << command << " from "
//					<< getUserFromTCPMap(peer)->getName() << endl;
//				cout<<"Dispatcher: COMMAND FROM PEER:"<<command<<endl;
			if (command > 0) {
//					cout<<"Dispatcher: GOT COMMAND FROM PEER"<<endl;
				peeread = true;
			}
			int len = 0;
			char buff[100];
			int rc = 0;
			string data;

			string name;

			string pass;

			string accept;

			string allUsers;

			string score;

			string oppPort;

			int oppPortInt;

			User* oppUser;

			int intScore;

			string scoreUpdate;

			map<TCPSocket*, User*>::iterator pos;

			User *user, *user2, *user3, *u, *opp, *opp2;
			User *player1, *player2;
			Guard guard(&mutex);

			int success;
			switch (command) {

			case REGISTER:
				if (getUserFromTCPMap(peer)!=NULL)
				{
					sendCommand(peer,OUTPUT,"You already registered!");
					break;
				}
				else{
					rc = peer->recv((char*) &len, 4); //recv msg length
					if (rc < 0) {
						cerr << "Dispatcher: Fail to read command from socket"<< endl;
					}
					len = ntohl(len);
					rc = peer->recv(buff, len); //recv msg
					if (rc < 0) {
						cerr << "Dispatcher: Fail to read command from socket"
								<< endl;
					}
					buff[len] = '\0';
					data = buff;

					name = data.substr(0, data.find_first_of(":"));
					pass = data.substr(data.find_first_of(":") + 1);


					um->registerUser(name, pass);
				}
				break;

			case LOGIN:
				if (getUserFromTCPMap(peer)!=NULL && getUserFromTCPMap(peer)->isLoggedIn())
				{
					sendCommand(peer, OUTPUT, "You already logged in!");
					break;
				}
				else if (getUserFromTCPMap(peer)==NULL)
				{
					rc = peer->recv((char*) &len, 4); //recv msg length
					if (rc < 0) {
						cerr << "Dispatcher: Fail to read command from socket"
								<< endl;
					}
					len = ntohl(len);
					rc = peer->recv(buff, len); //recv msg
					if (rc < 0) {
						cerr << "Dispatcher: Fail to read command from socket"
								<< endl;
					}
					buff[len] = '\0';
					data = buff;

					name = data.substr(0, data.find_first_of(":"));
					pass = data.substr(data.find_first_of(":") + 1);

					success = um->loginUser(name, pass);
					if (success == 0) {
						sendCommand(peer, OUTPUT, "LOGIN SUCCESS");
						user = new User(name, pass);
						user->setTcp(peer);
						user->setLoggedIn(true);
						tcpMap->insert(make_pair(peer, user));
						addLoggedPeer(peer);
					} else if (success == WRONG_USER) {
						sendCommandOnly(peer, WRONG_USER);
					} else if (success == WRONG_PASS) {
						sendCommandOnly(peer, WRONG_PASS);
					}

					for (pos = (*tcpMap).begin(); pos != (*tcpMap).end(); ++pos) {
						User* user = pos->second;
						if (!user->isOnGame()) //if the user is not playing right now - print the user
						{
							allUsers += user->getName() + "\n";
						}
					}
					cout << allUsers << endl;
				}
				break;

			case SHOW_USERS:
				if (getUserFromTCPMap(peer)!=NULL && getUserFromTCPMap(peer)->isLoggedIn())
				{
					printLoggedUsers(peer);
				}
				else {
					sendCommand(peer, OUTPUT, "Please LOGIN before doing anything else!");
				}
				break;
			case OPEN_MATCH_RANDOM:
				if (getUserFromTCPMap(peer)!=NULL && getUserFromTCPMap(peer)->isLoggedIn())
				{
					user2 = getUserFromTCPMap(peer);
					if (user2 != NULL) {
						if (tcpMap->size() == 1) {
							sendCommand(peer, OUTPUT, "You're all alone! Wait for more players to join!");
							break;
						}
						rc = peer->recv((char*) &len, 4); //recv msg length
						if (rc < 0) {
							cerr << "Dispatcher: Fail to read command from socket"
									<< endl;
						}
						len = ntohl(len);
						rc = peer->recv(buff, len); //recv msg
						if (rc < 0) {
							cerr << "Dispatcher: Fail to read command from socket"
									<< endl;
						}
						buff[len] = '\0';
						data = buff;

						opp = tcpMap->begin()->second;
						if (opp->getName() == user2->getName()) { //The same user
							r = r % tcpMap->size();
							iter = tcpMap->begin();
							for (int var = 0; var < r; ++var) {
								iter++;
							}
							opp = iter->second; //Random user
						}

						//If one or more of the players is already in the game
						if (user2->isOnGame() || opp->isOnGame()) {
							sendCommand(user2->getTcp(), OUTPUT,
									"You or your opponent are already in a game!!! Exit the game first!");
							break;
						}

						accept = "Hi " + opp->getName()
								+ ", do you want to play with: " + user2->getName()
								+ "? Enter: 6 to accept, 7 to decline: ";
						sendCommand(opp2->getTcp(),INVITATION, data.c_str());
						sendCommand(opp->getTcp(), OUTPUT, accept.c_str());

						player1 = user2;
						player2 = opp;

					}
				}
				else {
					sendCommand(peer, OUTPUT, "Please LOGIN before doing anything else!");
				}

				break;

			case OPEN_MATCH_WITH_USER:
				user3 = getUserFromTCPMap(peer);

				//CHECK IF THE USER IS LOGGED IN
				if (getUserFromTCPMap(peer)!=NULL && getUserFromTCPMap(peer)->isLoggedIn())
				{
					rc = peer->recv((char*) &len, 4); //recv msg length
					if (rc < 0) {
						cerr << "Dispatcher: Fail to read command from socket"
								<< endl;
					}
					len = ntohl(len);
					rc = peer->recv(buff, len); //recv msg
					if (rc < 0) {
						cerr << "Dispatcher: Fail to read command from socket"
								<< endl;
					}
					buff[len] = '\0';
					data=buff;
					name = data.substr(0, data.find_first_of(":"));
					data = data.substr(data.find_first_of(":") + 1);
					//Check that the user didn't ask to play with himself
					if (name == user3->getName()) {
						cout << "You can't play with yourself!" << endl;
						break;
					}
					//Search the map for the opp's name
					map<TCPSocket*, User*>::iterator pos;
					for (pos = (*tcpMap).begin(); pos != (*tcpMap).end();
							++pos) {
						u = pos->second;
//							cout<<u;
						if (u->getName() == name)
							opp2 = u;
					}
					if (opp2 == NULL) {
						sendCommand(user3->getTcp(), OUTPUT,
								"Opponent's name not found!");
						break;
					}

					if (user3->isOnGame() || opp2->isOnGame()) {
						sendCommand(user3->getTcp(), OUTPUT,
								"You or your opponent are already in a game!!! Exit the game first!");
						break;
					}

					accept = "Hi " + opp2->getName()
							+ ", do you want to play with: " + user3->getName()
							+ "? Enter: 6 to accept, 7 to decline: ";

					if (opp2->getTcp() != NULL)
					{
						sendCommand(opp2->getTcp(), INVITATION, data.c_str());
						sendCommand(opp2->getTcp(), OUTPUT, accept.c_str());
					}

					player1 = user3;
					player2 = opp2;
				}
				else {
					sendCommand(peer, OUTPUT, "Please LOGIN before doing anything else!");
				}
				break;
			case ACCEPT_MATCH:
				//CHECK IF THE USER IS LOGGED IN
				if (getUserFromTCPMap(peer)!=NULL && getUserFromTCPMap(peer)->isLoggedIn())
				{
					if (player1 != NULL && player2 != NULL) {
						cout<<"accept match"<<player1<<endl;
						sendCommand(player1->getTcp(),OUTPUT,"Opponent has accepted the challenge!");
					}
//						sendCommand(peer,OUTPUT,"Opponent has accepted the challenge!");
//						handler->openGameSession(player1, player2);
//					} else {
//						cerr << "One or two of players is null!" << endl;
//					}
				}
				else {
					sendCommand(peer, OUTPUT, "Please LOGIN before doing anything else!");
				}
				break;
			case DECLINE_MATCH:
				//CHECK IF THE USER IS LOGGED IN
				if (getUserFromTCPMap(peer)!=NULL && getUserFromTCPMap(peer)->isLoggedIn())
				{
					cout<<"okay"<<endl;
					sendCommand(player2->getTcp(), OUTPUT, "Okay, no match today.");
					sendCommand(player1->getTcp(), OUTPUT,"Your opponent has refused to play with you. Please choose again.");
				}
				else {
					sendCommand(peer, OUTPUT, "Please LOGIN before doing anything else!");
				}
				break;

			case OPP_SCORE:
				rc = peer->recv((char*) &len, 4); //recv msg length
				if (rc < 0) {
					cerr << "Dispatcher: Fail to read command from socket"
							<< endl;
				}
				len = ntohl(len);
				rc = peer->recv(buff, len); //recv msg
				if (rc < 0) {
					cerr << "Dispatcher: Fail to read command from socket"
							<< endl;
				}
				buff[len] = '\0';
				oppPort=buff;
				score = oppPort.substr(0, oppPort.find_first_of(":"));
				oppPort = oppPort.substr(oppPort.find_first_of(":") + 1);

				intScore = atoi(score.c_str());
				intScore = ntohl(intScore);


				oppPortInt = atoi(oppPort.c_str()); //check if works
				oppPortInt = ntohl(oppPortInt);

				//get user out of his port
				oppUser = getUserFromTCPMapByPort(oppPortInt);
				if (oppUser==NULL)
					cerr<<"The opponent with the score: "<<intScore<<" is null!"<<endl;

				fm->setScore(oppUser,intScore);
				scoreUpdate = "Your score was updated! You got: "+score+" points!";
				sendCommand(opp2->getTcp(), OUTPUT, scoreUpdate.c_str());
				break;

			case EXIT_SERVER:
				disconnectUser(peer);
				break;
			} //switch
		} //else
//		cout<<"Dispatcher: !closed"<<endl;
	} //while
	cout << "Dispatcher: End of dispatcher run loop" << endl;
}

void Dispatcher::printLoggedUsers(TCPSocket* peer) {
//	waitForThread();
	User* connectedUser = getUserFromTCPMap(peer);
	cout << "Printing logged in users: " << endl;
	map<TCPSocket*, User*>::iterator pos;
	cout << "MAP SIZE: " << tcpMap->size() << endl;
	if (tcpMap->size() < 2) {
		sendCommand(peer, OUTPUT,
				"You are alone! Wait for more users to connect...");
	} else {
		string allUsers;
		for (pos = (*tcpMap).begin(); pos != (*tcpMap).end(); pos++) {
			User* user = pos->second;
			if (!user->isOnGame()
					&& user->getName() != connectedUser->getName()) //if the user is not playing right now - print the user
							{
//				cout << "printing map: " << endl;
//				cout << user->getName() << endl;
				allUsers += user->getName(); // + "\n"
			}
		}
		cout << allUsers << endl;

		int res = sendCommand(peer, USERLIST, allUsers.c_str());
		if (res < 4) {
			cout << "Dispatcher: Could not print names! " << endl;
		}
	}
}

int Dispatcher::sendCommand(TCPSocket* peer, int command, const char* buff) {
	int cmdNet = htonl(command);
	int res = peer->write((char*) &cmdNet, 4); //the command int from the protocol
	if (res < 4)
		cerr << "Can't send" << endl;
	int len = strlen(buff);
	int lenNet = htonl(len);
	res = peer->write((char*) &lenNet, 4); //the length of the message
	if (res < 4)
		cerr << "Can't send" << endl;
	res = peer->write(buff, len); //the message
	if (res < 4) {
		cout << "Dispatcher: Could not print names! " << endl;
	}
	return res;
}

int Dispatcher::readCommand(TCPSocket* peer) {
	int rcvCmd = 0;
	int rc = peer->recv((char*) &rcvCmd, 4);
	while (rc != 4) {
		rc = peer->recv((char*) &rcvCmd, 4);
//		cerr<<"Dispatcher: fail to read command from peer"<<endl;
//		return -1;
	}
	return ntohl(rcvCmd);
}

User* Dispatcher::getUserFromTCPMap(TCPSocket* sock) {
	map<TCPSocket*, User*>::iterator iter;
	iter = tcpMap->find(sock);
	if (iter != (*tcpMap).end()) {
		User* user = new User(iter->second);
		return user;
	} else {
//		cerr << "getUserFromTCPMap: No such user" << endl;
		return NULL;
	}
}

User* Dispatcher::getUserFromTCPMapByPort(int port) {
	map<TCPSocket*, User*>::iterator iter;
	iter = (*tcpMap).begin();
	while (iter != (*tcpMap).end()) {
		if (iter->first->getPort() == port)
		{
			User* user = new User(iter->second);
			return user;
		}
		iter++;
	}
//		cerr << "getUserFromTCPMap: No such user" << endl;
	return NULL;
}

bool Dispatcher::removePeerFromVector(TCPSocket* peer) {
	if (peer == NULL) {
		cout << "Dispatcher: PEER IS NULL!" << endl;
		return false;
	}
	int i = 0;
	tSockets::iterator iter = peers->sockets.begin();
	for (; iter != peers->sockets.end(); iter++) {
		if (*iter != NULL) {
			TCPSocket* curr = *iter;
			if (curr->getIP() == peer->getIP()
					&& curr->getPort() == peer->getPort()) {
				cout << "Dispatcher: Peer to delete: "
						<< peers->sockets[i]->getPort() << endl;
				Guard guard(&mutex);
				peers->sockets.erase(peers->sockets.begin() + i);
				if (peers->sockets.size() == 0) {
					peersOn = false;
				}
				return true;
			}
			i++;
		} else {
			cout << "Dispatcher: iter null" << endl;
		}
	}
	return false;

}

void Dispatcher::updateScore(User* user, int score) {
	Guard guard(&mutex);
	//update file with the new score of user (replace old user)
	fm->setScore(user, score);
	//now update the user itself
	user->setScore(score);
	//update the map
	map<TCPSocket*, User*>::iterator it = tcpMap->find(user->getTcp());
	if (it != tcpMap->end()) {
		it->second = user;
	}
}

void Dispatcher::disconnectUser(TCPSocket* peer) {
	Guard guard(&mutex);
	User* u = getUserFromTCPMap(peer);
	int i = 0;
	if (u!=NULL)
	{
		if (u->isOnGame()) {
			sendCommand(peer, OUTPUT, "You lost the game, bye!");
//			handler->closeGame(u);
			//TODO: CLOSE THE GAME
		} else
			sendCommand(peer, OUTPUT, "BYE!");

		for (map<TCPSocket*, User*>::iterator it = tcpMap->begin();
				it != tcpMap->end(); it++) {
			if (it->second == u) {
				tcpMap->erase(it);
				break;
			}
		}
		bool removed = removePeerFromVector(peer);
		if (removed)
			cout << "Removed " << peer << endl;
		else
			cerr << "Peer wasn't removed" << endl;
	}

	else {

			tSockets::iterator iter = waitingPeers->sockets.begin();
			for (; iter != waitingPeers->sockets.end(); iter++) {
				if (*iter != NULL) {
					TCPSocket* curr = *iter;
					if (curr->getIP() == peer->getIP() && curr->getPort() == peer->getPort()) {
						cout << "Dispatcher: Peer to delete: "
								<< waitingPeers->sockets[i]->getPort() << endl;
						Guard guard(&mutex);
						waitingPeers->sockets.erase(waitingPeers->sockets.begin() + i);
						peers->sockets.push_back(peer);
						if (waitingPeers->sockets.size() == 0) {
							peersOn = false;
						}
					}
					i++;
				}
				else {
					cout << "Dispatcher: iter null" << endl;
				}
			}
	}
	delete (u);
	delete (peer);
}

Dispatcher::~Dispatcher() {
	//delete peers+waitingpeers from memory
	//delete handler;
	//delete tcpMap
	//delete fm+um
}

/*



 void Dispatcher::run(){
 //while(peersOn){
 //	bool peeread = false; //make sure the command is read only one time
 //	while (peersOn&& !peeread){
 while(!closed){
 //		cout<<"Dispatcher: size of peers: "<<peers->sockets.size()<<endl;
 MTCPListener mtcp;

 mtcp.addAll(peers->sockets);
 mtcp.addAll(waitingPeers->sockets);

 peers->sockets.clear();
 waitingPeers->sockets.clear();

 //now we update the original peers list

 peers->sockets = mtcp.sockets;

 //		if (peersOn&& !peeread){
 TCPSocket* peer = mtcp.listen(4);

 if(!peer){ //timeout without any peer ready
 //				cout<<"Dispatcher: there is no ready peer"<<endl;
 continue;
 }
 else
 {
 cout<<"Dispatcher: peer: "<<peer->getPort()<<" is now connected!"<<endl;
 int command = readCommand(peer);
 //				cout<<"Dispatcher: COMMAND FROM PEER:"<<command<<endl;
 if(command>0){
 //					cout<<"Dispatcher: GOT COMMAND FROM PEER"<<endl;
 peeread = true;
 }
 int len = 0;
 string ip;
 char buff[100];
 int rc = 0;
 int port = 0;
 string data;
 switch(command){

 case OPEN_SESSION_WITH_PEER:
 cout<<"Dispatcher: Got open session command"<<endl;
 waitForThread();
 //					cout<<"Dispatcher: Done waiting!"<<endl;
 rc = peer->recv((char*)&len,4); //recv msg length
 if(rc<0){
 cerr<<"Dispatcher: Fail to read command from socket"<<endl;
 }
 len = ntohl(len);
 rc = peer->recv(buff,len); //recv msg
 if(rc<0){
 cerr<<"Dispatcher: Fail to read command from socket"<<endl;
 }
 buff[len] = '\0';
 data = buff;


 ip = data.substr(0,data.find_first_of(":"));
 port = atoi((data.substr(data.find_first_of(":") + 1)).c_str());
 int index = getPeerIndex(ip,port);
 if(index!=-1){
 int com = handler->managePeerSession(peer,peers->sockets[index]);
 if(com==SESSION_ESTABLISHED){
 cout<<"Dispatcher: SESSION_ESTABLISHED"<<endl;
 sendFeedback(peer,6);
 removePeer(peer); //remove the peer that requested to open session
 sendFeedback(peers->sockets[index],6);
 removePeer(peers->sockets[index]);//remove the other peer
 }
 else if(com==SESSION_REFUSED){
 cout<<"Dispatcher: SESSION_REFUSED"<<endl;
 sendFeedback(peer,5);
 }
 }
 else
 {
 cout<<"Dispatcher: Peer not found!"<<endl;
 sendFeedback(peer,5);
 }
 break;
 }
 }
 //		}
 //		cout<<"Dispatcher: !closed"<<endl;
 }
 cout<<"Dispatcher: End of dispatcher run loop"<<endl;
 }
 int Dispatcher::getPeerIndex(string ip,int port){
 return peers->getSocketIndex(ip,port);
 }

 int Dispatcher::sendFeedback(TCPSocket* peer, int command){
 int cmdNet = htonl(command);
 int res = peer->write((char*)&cmdNet,4); //the command int from the protocol

 if (res<4)
 {
 cout<<"Dispatcher: Could not send feedback! "<<command<<endl;
 return -1;
 }
 return res;
 }

 int Dispatcher::readCommand(TCPSocket* peer){
 int rcvCmd = 0;
 int rc = peer->recv((char*)&rcvCmd,4);
 if(rc!=4){
 cerr<<"Dispatcher: fail to read command from peer"<<endl;
 return -1;
 }
 return ntohl(rcvCmd);
 }

 void Dispatcher::close(){
 closed = true;
 Guard guard(&mutex);
 if(!peers->sockets.empty())
 peers->sockets.clear();
 }
 a
 Dispatcher::~Dispatcher() {
 cout<<"Dispatcher: Dispatcher closed!"<<endl;
 }

 */
