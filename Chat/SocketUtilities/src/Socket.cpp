/*
 * Socket.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: user
 */

#include "Socket.h"

//the default constructor creates tcp socket
Socket::Socket()
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int optval;

    optval = 1;

    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
}

char *Socket::getIP() const
{
    struct sockaddr_in localAddress;
    unsigned int addressLength = sizeof(localAddress);
    bzero(&localAddress, sizeof(localAddress));
    getsockname(fd, (struct sockaddr *)&localAddress, &addressLength);
    return inet_ntoa(localAddress.sin_addr);
}

//the default constructor creates socket according to the type received as argument
Socket::Socket(int type)
{
    socket_fd = socket(AF_INET, type, 0);
    int optval;

    optval = 1;

    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    if (socket_fd < 0)
    {
        perror("Socket: Error: Didn't create a socket.");
    }
}
//will be executed from server socket
//socket_fd - the fd of the server
//s_in - the struct that holds the server details, the argument port is the server port
int Socket::bind(int port)
{
    struct sockaddr_in s_in;
    bzero((char *)&s_in, sizeof(s_in));
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons(port);

    int optval = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    fflush(stdout);
    if (::bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)) < 0)
    {
        perror("Socket: Error: socket didn't bind.");
        return -1;
    }
    return 1;
}

//will be executed from client socket
//in tcp socket - the arguments ip,port are of the server
//in udp socket - the arguments ip,port are of the other peer we want to send msg to
//socket fd - the client fd
int Socket::connect(string ip, int port)
{

    bzero(&remote, sizeof(remote));

    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(ip.data());
    remote.sin_port = htons(port);

    if (::connect(socket_fd, (struct sockaddr *)&remote, sizeof(remote)) < 0)
    {
        perror("Socket: client: Error: Didn't connect to server.");
        return -1;
    }
    return 1;
}
//read from the current socket
int Socket::read(char *buff, int size)
{
    return ::read(socket_fd, buff, size);
}
//write from the current socket
int Socket::write(const string &msg)
{
    return ::write(socket_fd, msg.c_str(), msg.size());
}
//close the current socket
void Socket::close()
{
    ::close(socket_fd);
}

Socket::~Socket()
{
}
