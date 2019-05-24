/*
 * GameManager.h
 *
 *  Created on: Sep 25, 2017
 *      Author: eontorch
 */

#ifndef SRC_GAMEMANAGER_H_
#define SRC_GAMEMANAGER_H_

//#include "ClientHandler.h"
#include "User.h"
#include "Game.h"

namespace std {

class GameManagerHandler{
public:
	virtual void updateScore(User* user, int score) = 0;
	virtual ~GameManagerHandler();
};

class GameManager {
	GameManagerHandler* handler;
public:
	GameManager(GameManagerHandler* handler);
	int openGameSession(User* user1, User* user2);
	void closeGameWithUser(User* user);
	void updateScoreOfUser(User* user, int score);

	void sendCommandToClient(TCPSocket* peer, int command, const char* buff);

	virtual ~GameManager();
};

} /* namespace std */

#endif /* SRC_GAMEMANAGER_H_ */
