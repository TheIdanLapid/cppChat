/*
 * Game.h
 *
 *  Created on: Sep 25, 2017
 *      Author: eontorch
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include "User.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

namespace std {

class Game : MThread {
	UDPSocket* udp1;
	UDPSocket* udp2;
	TCPSocket* tcp2;
public:
	Game();
	void run();
	Game(User* user1, User* user2);
	virtual ~Game();
};

} /* namespace std */

#endif /* SRC_GAME_H_ */
