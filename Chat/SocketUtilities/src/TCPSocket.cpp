/*
 * TCPPSocket.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: Idan & Shdema
 */

#include "TCPSocket.h"
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;

//tcp server socket
//port - the server port
TCPSocket::TCPSocket(int port) : Socket(SOCK_STREAM)
{
    if (port != 9999)
    {
        bind(port);
    }
}

/**
 * tcp client socket
 * ip,port - the server ip,port
 */
TCPSocket::TCPSocket(string peerIp, int port) : Socket(SOCK_STREAM)
{
    if (port != 9999)
    {
        connect(peerIp, port);
    }
}

//a new tcp socket that is created after the server accept the client
TCPSocket::TCPSocket(int socket_fd, struct sockaddr_in local, struct sockaddr_in remote)
{
    this->socket_fd = socket_fd;
    this->local = local;
    this->remote = remote;
}

//the method is called from tcp server
// a new tcp socket with the server fd, local - server, remote - client
//listen to it's own socket (after binding)
//accept: socket_fd - the server fd, accept the remote struct (client) -??
TCPSocket *TCPSocket::listenAndAccept()
{
    ::listen(socket_fd, 1);
    size_t len = sizeof(remote);
    bzero((char *)&this->remote, sizeof(this->remote));

    int fd = ::accept(socket_fd, (struct sockaddr *)&remote, (unsigned int *)&len);
    if (fd < 0)
    {
        perror("TCPSocket: Error creating socket");
        close();
        return NULL;
    }
    TCPSocket *tcp = new TCPSocket(fd, this->local, this->remote);
    return tcp;
}

int TCPSocket::recv(char *buffer, int length)
{
    int recv = ::read(socket_fd, buffer, length);
    return recv;
}

int TCPSocket::send(const string &msg)
{
    int send = ::write(socket_fd, msg.c_str(), msg.size());
    return send;
}

int TCPSocket::write(const char *msg, int size)
{
    return ::write(socket_fd, msg, size);
}

int TCPSocket::write(const string &msg)
{
    Socket::write(msg);
    return 1;
}

int TCPSocket::getPort()
{
    return (int)ntohs(remote.sin_port);
}

string TCPSocket::getIP()
{
    return inet_ntoa(remote.sin_addr);
}

string TCPSocket::fromAddr()
{
    return inet_ntoa(local.sin_addr);
}
int TCPSocket::getSocketFd() const
{
    return this->socket_fd;
}

TCPSocket::~TCPSocket()
{
}
