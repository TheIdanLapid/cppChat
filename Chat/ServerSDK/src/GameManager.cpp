/*
 * GameManager.cpp
 *
 *  Created on: Sep 25, 2017
 *      Author: eontorch
 */

#include "GameManager.h"

namespace std
{

GameManager::GameManager(GameManagerHandler *handler)
{
    this->handler = handler;
}

int GameManager::openGameSession(User *user1, User *user2)
{
    Game *game = new Game(user1, user2);
    return 1;
}

void GameManager::closeGameWithUser(User *user)
{
}

void GameManager::updateScoreOfUser(User *user, int score)
{
    handler->updateScore(user, score);
}

void sendCommandToClient(TCPSocket *peer, int command, const char *buff)
{
}

GameManager::~GameManager()
{
}

} /* namespace std */
