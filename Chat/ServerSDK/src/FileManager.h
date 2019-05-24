/*
 * FileManager.h
 *
 *  Created on: Sep 4, 2017
 *      Author: idan
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include "User.h"
#include "File.h"
#include "Guard.h"
#include <math.h>
#include <algorithm>

using namespace std;

class FileManager {
	pthread_mutex_t mutex;
	File f;
	File* file;
	std::ifstream fileIn;
	map<string,pair<string,int> >* usersMap; //Maps username to pair<password,score>

	//Helper funcs
	int readLineIntoMap(map<string, pair<string, int> >* usersMap, char* userString);
	int howManyDigits(int x);
	int findCharNthPosInStr(char *haystack, char needle, int nth);
	int readUntilComa(char* str, int* numIn, string& strIn);
	string hashpass(string password);
public:
	FileManager();

	string readUsers(); //Reads a user from the users file
	int readScore(string name); //Reads a user's accumulated score
	User* getUserFromMap(string name);

//	void writeUser(User user, map<string, pair<string, int> >* usersMap); //Writes a new user to the users file
	void writeUser(User* user);
	void updateFileFromMap();
	void setScore(User* user, int score); //Adds a new score to the user's accumulated score
	void clearFile();
	void setUsersMap(map<string, pair<string, int> >* usersMap);
	virtual ~FileManager();
	map<string, pair<string, int> >* getUsersMap();
};

#endif /* FILEMANAGER_H_ */
