/*
 * FileBase.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: user
 */

#include "FileBase.h"

#include <unistd.h>


FileBase::FileBase():fd(-1) {

}
int FileBase::write(const char* buffer, int length){
	return ::write(fd, buffer, length);
}

int FileBase::read(char* buffer, int length){
	return ::read(fd, buffer, length);
}

FileBase::~FileBase() {
}

