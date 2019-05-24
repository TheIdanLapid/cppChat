/*
 * File.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: user
 */

#include "File.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

File::File(){};

File::File(string path) {
	fd = open(path.data(), O_CREAT|O_RDWR  , 0666);
	if (fd == -1){
		perror("Cannot open output file\n");
	}
}

void File::closeOpenAndClear(string path) {
	close();
	fd = open(path.data(), O_CREAT|O_RDWR|O_TRUNC  , 0666); //Clears the file content
	if (fd == -1){
		perror("Cannot open output file\n");
	}
}

void File::close() {
	::close(fd);
}

File::~File() {
}

