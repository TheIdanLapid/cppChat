/*
 * ServerSDK.cpp
 *
 *  Created on: Sep 24, 2017
 *      Author: user
 */

#include "ServerSDK.h"

ServerSDK::ServerSDK()
{
    acceptor = new Acceptor(this);
    dispatcher = new Dispatcher(this);
    gameManager = new GameManager(this);
}

void ServerSDK::closeGame(User *user)
{
    gameManager->closeGameWithUser(user);
}

void ServerSDK::updateScore(User *user, int score)
{
    dispatcher->updateScore(user, score);
}

void ServerSDK::printUsers(TCPSocket *peer)
{
    dispatcher->printLoggedUsers(peer);
    cout << "Finished printing users" << endl;
}

void ServerSDK::handlePeer(TCPSocket *peer)
{
    dispatcher->addPeer(peer);
    cout << "Peer " << peer->getPort() << " moved to dispatcher!" << endl;
}

int ServerSDK::openGameSession(User *user1, User *user2)
{
    int success = gameManager->openGameSession(user1, user2);
    return success;
}

DispatcherHandler::~DispatcherHandler()
{
}

AcceptorHandler::~AcceptorHandler()
{
}

GameManagerHandler::~GameManagerHandler()
{
}

ServerSDK::~ServerSDK()
{
}

void ServerSDK::start()
{
    acceptor->start();
    sleep(3);
    dispatcher->start();
}
