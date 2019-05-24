
#include <iostream>
using namespace std;
#include "User.h"
#include "Protocol.h"
#include <vector>
#include "netinet/in.h"
#include "ServerSDK.h"
#include "Dispatcher.h"
#include "Acceptor.h"

void printMap(FileManager* fm) {
	uMap* map = fm->getUsersMap(); //uMap is defined in Protocol.h as: map<string,pair<string,int> >
	uMap::iterator pos;
	for (pos = (*map).begin(); pos != (*map).end(); ++pos) {
		cout << "key: " << pos->first << " values: pass: " << pos->second.first << " score: " <<pos->second.second << endl;
	}
}

int main() {
	ServerSDK* server = new ServerSDK();
	server->start();

	while (true){}
	cout<<"End of TEST"<<endl;

	return 0;
}
