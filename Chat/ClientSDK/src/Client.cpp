/*
 * Client.cpp
 *
 *  Created on: Oct 4, 2017
 *      Author: user
 */

#include "Client.h"

using namespace std;

Client::Client(ClientHandler *handler)
{
    number = 0;
    guess = 0;
    oppBuf = 0;
    this->handler = handler;
    socket = NULL;
    udp = NULL;
    opp = NULL;
    connected = false;
    closed = false;
    onGame = false;
    logged = false;
    opponent = "";
    mutex = PTHREAD_MUTEX_INITIALIZER;
    oppPort = 0;
    udpl = NULL;
}

UDPSocket *Client::getUDPSocket()
{
    return udp;
}

int Client::sendCommandToServer(TCPSocket *socket, int com, string rest)
{
    int cmdNet = htonl(com);
    int res = socket->write((char *)&cmdNet, 4); //the command int from the protocol
    if (res < 4)
        return -1;
    int len = rest.length();
    int lenNet = htonl(len);
    res = socket->write((char *)&lenNet, 4); //the length of the message
    if (res < 4)
        return -1;
    res = socket->write(rest.c_str(), len); //the message
    if (res < 4)
        return -1;
    return 1;
}

int Client::sendCommandToServer(TCPSocket *socket, int com)
{
    int cmdNet = htonl(com);
    int res = socket->write((char *)&cmdNet, 4); //the command int from the protocol
    if (res < 4)
        return -1;
    return 1;
}

void Client::registerUser(string user, string pass)
{
    sendCommandToServer(socket, REGISTER, user + ":" + pass);
}

void Client::login(string user, string pass)
{
    sendCommandToServer(socket, LOGIN, user + ":" + pass);
}

void Client::run()
{
    while (connected)
    {
        string data;
        int read = readcommand(data);
        if (read < 0)
        {
            cout << "TCP Client disconnected!" << endl; //TODO: check if client was on game, delete client and notify others
        }
    }
}

bool Client::connect(string ip)
{
    socket = new TCPSocket(ip, SERVER_PORT);
    cout << "getPort: " << getPort() << endl;
    cout << "Socket get port: " << socket->getPort() << endl;
    if (socket > 0)
    {
        connected = true;
        udp = new UDPSocket(getPort());
        udpl = new UDPListener(udp); //build and start the listener
        start();
        return true;
    }
    return false;
}

int Client::startTheGame()
{
    opp = new UDPSocket(oppPort);
    int connect = udp->connect(oppIp, oppPort);
    if (connect != 1)
        cerr << "Could not connect udp socket!!!" << endl;
    else
        cout << socket->getPort() << " is connected with: " << oppPort << endl;

    //wait until user has guessed a number in CLI
    cout << "getPort: " << getPort() << endl;
    cout << "Socket get port: " << socket->getPort() << endl;
    while (guess == 0)
        sleep(2);
    cout << "StartTheGame: " << guess << endl;
    guess = htonl(guess);
    stringstream strs;
    strs << guess;
    string guessStr = strs.str();

    udp->write(guessStr);
    string oppGuess = "";
    do
    {
        sleep(1);
        oppGuess = udpl->getMSG();
        cout << "oppGuess= " << oppGuess << endl;
    } while (oppGuess == "");

    oppBuf = atoi(oppGuess.c_str());
    oppBuf = ntohl(oppBuf);

    cout << "Opponent's Guess: " << oppBuf << endl;

    int score = 10 - abs(oppBuf - number);
    cout << "Opponent's Score: " << score << endl;

    score = htonl(score);

    strs << score;
    string scoreStr = strs.str();
    scoreStr += ":" + portStrOpp;

    sendCommandToServer(socket, OPP_SCORE, scoreStr);
    return 1;
}

bool Client::openGame(string name)
{
    Guard guard(&mutex);
    //send command openGame to the server
    int port = getPort();
    int netPort = htonl(port);
    stringstream strs;
    strs << netPort;
    string temp_str = strs.str();

    string ipPort = string(udp->getIP()) + ":" + temp_str;

    string fullMsg = name + ":" + ipPort;
    int success = sendCommandToServer(socket, OPEN_MATCH_WITH_USER, fullMsg.c_str());
    if (success == -1)
        return false;
    return true;
}

bool Client::openGameRandom()
{
    Guard guard(&mutex);
    //send command openGame to the server
    int port = getPort();
    int netPort = htonl(port);
    stringstream strs;
    strs << netPort;
    string temp_str = strs.str();

    string ipPort = string(udp->getIP()) + ":" + temp_str;
    int success = sendCommandToServer(socket, OPEN_MATCH_RANDOM, ipPort.c_str());
    if (success == -1)
        return false;
    return true;
}

void Client::closeGame()
{
    Guard guard(&mutex);
    int success = sendCommandToServer(socket, EXIT_MATCH);
    if (success == -1)
        cerr << "Couldn't close msg" << endl;
    else
    {
        onGame = false;
        closed = true;
    }
}

void Client::disconnect()
{
    Guard guard(&mutex);
    int success = sendCommandToServer(socket, EXIT_SERVER);
    if (success == -1)
        cerr << "Can't disconnect" << endl;
    else
    {
        if (onGame)
            onGame = false;
        connected = false;
    }
}

void Client::displayUsers()
{
    sendCommandToServer(socket, SHOW_USERS);
}

int Client::readcommand(string &data)
{
    string ip, name;
    const char *portChar;
    int portInt;
    int portNet;
    int rcvCmd = 0;
    int len = 0;
    char buff[100];
    sleep(2);
    int rc = socket->recv((char *)&rcvCmd, 4);
    if (rc != 4)
    {
        return -1;
    }
    rcvCmd = ntohl(rcvCmd);
    data = "";
    stringstream strs, strs1;

    switch (rcvCmd)
    {

    case USERLIST:
        rc = socket->recv((char *)&len, 4);
        if (rc < 0)
        {
            cerr << "fail to read command from socket" << endl;
            return -1;
        }
        len = ntohl(len);
        rc = socket->recv(buff, len);
        if (rc < 0)
        {
            cerr << "fail to read command from socket" << endl;
            return -1;
        }
        buff[len] = '\0';
        data = buff;
        userList = data;
        handler->setMSG(data);
        cout << userList << endl;
        break;
    case INVITATION:
        handler->setInvited(true);
        rc = socket->recv((char *)&len, 4);
        if (rc < 0)
        {
            cerr << "fail to read command from socket" << endl;
            return -1;
        }
        len = ntohl(len);
        rc = socket->recv(buff, len);
        if (rc < 0)
        {
            cerr << "fail to read command from socket" << endl;
            return -1;
        }
        buff[len] = '\0';
        data = buff;
        ip = data.substr(0, data.find_first_of(":"));
        portStrOpp = data.substr(data.find_first_of(":") + 1);

        portNet = atoi(portStrOpp.c_str());
        portInt = ntohl(portNet);

        cout << "oppIp: " << ip << endl;
        cout << "oppPort: " << portInt << endl;

        setOppIp(ip);
        setOppPort(portInt);
        break;
    case EXIT_MATCH:
        break;
    case OUTPUT:
        rc = socket->recv((char *)&len, 4);
        if (rc < 0)
        {
            cerr << "fail to read command from socket" << endl;
            return -1;
        }
        len = ntohl(len);
        rc = socket->recv(buff, len);
        if (rc < 0)
        {
            cerr << "fail to read command from socket" << endl;
            return -1;
        }
        buff[len] = '\0';
        data = buff;
        break;
    case WRONG_PASS:
        strs << WRONG_PASS;
        data = strs.str();
        break;
    case WRONG_USER:
        strs1 << WRONG_USER;
        data = strs1.str();
        break;
    }
    handler->setMSG(data);
    return 0;
}

void Client::acceptGame()
{
    sendCommandToServer(socket, ACCEPT_MATCH);
}
void Client::declineGame()
{
    sendCommandToServer(socket, DECLINE_MATCH);
}

int Client::getPort()
{
    socklen_t addressLength = sizeof(socket->remote);
    getsockname(socket->socket_fd, ((struct sockaddr *)&socket->remote), &addressLength);
    cout << (int)ntohs(socket->remote.sin_port) << endl;
    return ((int)ntohs(socket->remote.sin_port));
}
//udp message to other player
void Client::sendMessageUDP(string msg)
{
    if (onGame)
    {
        udp->write(msg);
    }
}

ClientHandler *Client::getHandler()
{
    return handler;
}

TCPSocket *Client::getSocket()
{
    return socket;
}
bool Client::isOnGame()
{
    return onGame;
}
bool Client::isConnected()
{
    return connected;
}
bool Client::isClosed()
{
    return closed;
}
bool Client::isLogged()
{
    return logged;
}

string Client::getUserList()
{
    return userList;
}

void Client::setLogged(bool val)
{
    logged = val;
}
void Client::setConnected(bool val)
{
    connected = val;
}
void Client::setClosed(bool val)
{
    closed = val;
}
void Client::setOnGame(bool val)
{
    onGame = val;
}

string Client::getOppIp()
{
    return oppIp;
}

int Client::getOppPort()
{
    return oppPort;
}

void Client::setOppIp(string ip)
{
    oppIp = ip;
}

void Client::setOppPort(int port)
{
    oppPort = port;
}

void Client::setGuess(int guess)
{
    this->guess = guess;
}

void Client::setNumber(int number)
{
    this->number = number;
}

Client::~Client()
{
    if (socket != NULL)
        delete socket;
}
