#include "ClientHandler.h"

using namespace std;

ClientHandler::ClientHandler(){
	lastMSG="";
	invited=false;
}

bool ClientHandler::isInvited() {
	return invited;
}

void ClientHandler::setInvited(bool inv) {
	invited = inv;
}

string ClientHandler::getMSG() {
	return lastMSG;
}
void ClientHandler::setMSG(string msg) {
	lastMSG=msg;
}

ClientHandler::~ClientHandler(){}
