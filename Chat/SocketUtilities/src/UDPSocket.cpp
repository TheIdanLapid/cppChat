/*
 * UDPSocket.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: Idan & Shdema
 */

#include "UDPSocket.h"
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
using namespace std;

UDPSocket::UDPSocket(int port) : Socket(SOCK_DGRAM)
{
    if (port != 9999)
        bind(port);
}

/**
 * 	- reads an incoming message from the UDP socket. the message is
 * 	copied into the given buffer up to the specified length.
 */
int UDPSocket::recv(char *buffer, int length)
{
    int a = sizeof(remote);
    return recvfrom(socket_fd, buffer, length, 0, (struct sockaddr *)&remote, (unsigned int *)&a);
}

/**
 * 	- sends the given message as a UDP message
 * 	to the given address specified by IP and port.
 *
 */
int UDPSocket::sendTo(string msg, string ip, int port)
{
    struct sockaddr_in dest;
    bzero((char *)&dest, sizeof(dest));

    dest.sin_family = (short)AF_INET;
    dest.sin_addr.s_addr = inet_addr(ip.data());
    dest.sin_port = htons(port);
    int cc = sendto(socket_fd, msg.data(), msg.length(), 0, (struct sockaddr *)&dest, sizeof(dest));

    if (cc < 0)
    {
        perror("UDPSocket: Error: Didn't send.");
        return (-1);
    }
    else
    {
        return cc;
    }
}
/**
 * - return the sender IP of the last received message.
 */
string UDPSocket::fromAddr()
{
    return (inet_ntoa(remote.sin_addr));
}

/**
 * 	- reply to an incoming message,
 * 	this method will send the given message as a UDP
 * 	message to the peer from which the last message was received.
 *
 */
int UDPSocket::reply(string msg)
{
    return (sendTo(msg, fromAddr(), ntohs(remote.sin_port)));
}

UDPSocket::~UDPSocket()
{
}
