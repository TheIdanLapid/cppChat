/*
 * FileManager.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: idan
 */

#include "FileManager.h"

FileManager::FileManager()
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
    f = File(PATH);
    file = &f;
    usersMap = new map<string, pair<string, int>>();
    fileIn.open(PATH);
}

string FileManager::readUsers()
{
    Guard guard(&mutex); //Guard the file to prevent r/w sync problems

    fileIn.seekg(0, ios::end);   //Put the "cursor" at the end of the file
    int length = fileIn.tellg(); //Find the position of the cursor

    if (length == 0)
    {
        cout << "Empty file" << endl;
        return "";
    }

    fileIn.seekg(0, ios::beg);

    int totalLen;       //Length of the whole line (except for the length itself and the space that comes after it)
    char space;         //A char to hold the space so it won't enter the string
    string fullLineStr; //The string that holds the entire line

    fileIn >> totalLen;
    fileIn.get(space);
    getline(fileIn, fullLineStr);

    char *fullLine = new char[fullLineStr.length() + 1];
    //Convert str to char*
    strcpy(fullLine, fullLineStr.c_str());

    char coma = ',';
    char userString[1024];
    char rest[1024];
    int indexOfSeperator;

    do
    {
        indexOfSeperator = findCharNthPosInStr(fullLine, coma, 5);
        //Put only the current user in userString
        strncpy(userString, fullLine, indexOfSeperator + 1);
        userString[indexOfSeperator + 1] = '\0';

        //Call readLineIntoMap() to write the users into the map
        readLineIntoMap(usersMap, userString);

        totalLen -= indexOfSeperator;
        //Cut userString from fullLine
        memcpy(rest, &fullLine[indexOfSeperator + 1], totalLen);
        strcpy(fullLine, rest);
        if ((string)fullLine == "")
            break;
        fullLine[totalLen] = '\0';
    } while (totalLen > 0 && indexOfSeperator != -1);
    fileIn.seekg(0, ios::beg);
    return fullLineStr;
}

int FileManager::findCharNthPosInStr(char *haystack, char needle, int n)
{
    string str = (string)haystack;        // where to work
    string find(1, needle);               // what to 'find'
    string::size_type i = str.find(find); // Find the first occurrence

    int j;
    for (j = 1; j < n && i != string::npos; ++j)
        i = str.find(find, i + 1); // Find the next occurrence

    if (j == n)
        return (i);
    else
        return (-1);
}

int FileManager::readLineIntoMap(map<string, pair<string, int>> *usersMap, char *userString)
{

    string none; //Does nothing, a dummy string for when we need to read int

    int nameLen;
    int nextComaIndex = readUntilComa(userString, &nameLen, none);

    userString += nextComaIndex + 1;

    string name;
    nextComaIndex = readUntilComa(userString, NULL, name);

    userString += nextComaIndex + 1;

    int passLen;
    nextComaIndex = readUntilComa(userString, &passLen, none);

    userString += nextComaIndex + 1;

    string pass;
    nextComaIndex = readUntilComa(userString, NULL, pass);

    userString += nextComaIndex + 1;

    int score;
    nextComaIndex = readUntilComa(userString, &score, none);

    userString += nextComaIndex + 1;

    pass = hashpass(pass); //Reverse password back to normal

    usersMap->insert(make_pair(name, make_pair(pass, score)));

    int howMuchHasBeenRead = howManyDigits(nameLen) + howManyDigits(passLen) + howManyDigits(score) + name.length() + pass.length() + 5; //The 5 is for the comas

    return howMuchHasBeenRead;
}

int FileManager::readUntilComa(char *str, int *numIn, string &strIn)
{
    string delimiter = ",";
    string s(str);
    size_t pos = 0;
    if ((pos = s.find(delimiter)) != std::string::npos)
    {
        if (numIn != NULL)
        {
            *numIn = atoi((s.substr(0, pos)).c_str());
        }
        else
            strIn = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
    }
    return pos;
}

int FileManager::howManyDigits(int x)
{
    return floor(log10(abs(x))) + 1;
}

void FileManager::writeUser(User *user)
{
    cout << "Writing user: " << user->getName() << endl;
    //First read the file and update the map
    string fullLine = readUsers();

    int prevLineLen = fullLine.length();

    Guard guard(&mutex);
    string line = "";
    char coma = ',';

    string name = user->getName();
    int nameLen = name.length();

    string pass = user->getPass();
    int passLen = pass.length();

    int score = user->getScore();

    char buff[1000];

    sprintf(buff, "%d", nameLen);

    line += string(buff);
    line += coma;
    line += name;
    line += coma;

    sprintf(buff, "%d", passLen);

    line += string(buff);
    line += coma;
    line += hashpass(pass);
    line += coma;

    sprintf(buff, "%d", score);

    line += string(buff);
    line += coma;

    sprintf(buff, "%d", (int)line.length() + prevLineLen); //Adds the previous line len to the new line len

    string lineLen = string(buff);

    string space = " ";

    file->closeOpenAndClear(PATH);

    file->write(lineLen.data(), lineLen.length()); //Writes the line's new length

    file->write(space.data(), 1); //Writes a space char

    line += fullLine; //Concat the new line to the prev

    file->write(line.data(), line.length());
    usersMap->insert(make_pair(user->getName(), make_pair(user->getPass(), user->getScore()))); //Insert the new user into map
}

int FileManager::readScore(string name)
{
    return 0;
}
//TODO: Check why score is updated from 0 every time (older score mitapeset)
void FileManager::setScore(User *user, int score)
{
    User *userTemp = getUserFromMap(user->getName());
    if (userTemp != NULL)
    {
        cout << "Updating Score for " << user->getName() << "... Old score: " << user->getScore() << endl;
        int nScore = user->getScore() + score; //The new score after we added the new one
        usersMap->erase(user->getName());      //Delete the old user from map
        user->setScore(nScore);
        usersMap->insert(make_pair(user->getName(), make_pair(user->getPass(), user->getScore()))); //Insert the new user into map
        cout << "New score: " << nScore << endl;
        updateFileFromMap(); //Update users.txt
    }
}

User *FileManager::getUserFromMap(string name)
{
    uMap::iterator iter;
    iter = usersMap->find(name);
    if (iter != usersMap->end())
    {
        User *user = new User(name, iter->second.first, iter->second.second);
        return user;
    }
    else
    {
        cerr << "getUserFromMap: No such user" << endl;
        return NULL;
    }
}

FileManager::~FileManager()
{
    f.close();
    fileIn.close();
}

map<string, pair<string, int>> *FileManager::getUsersMap()
{
    return usersMap;
}

void FileManager::setUsersMap(map<string, pair<string, int>> *usersMap)
{
    this->usersMap = usersMap;
}

string FileManager::hashpass(string password)
{
    std::reverse(password.begin(), password.end());
    return password;
}

void FileManager::updateFileFromMap()
{
    file->closeOpenAndClear(PATH);
    uMap::iterator pos;
    for (pos = usersMap->begin(); pos != usersMap->end(); ++pos)
    {
        cout << "key: " << pos->first << " values: pass: " << pos->second.first << " score: " << pos->second.second << endl;
        User *user = new User(pos->first, pos->second.first, pos->second.second);
        writeUser(user);
    }
}

void FileManager::clearFile()
{
    file->closeOpenAndClear(PATH);
}
