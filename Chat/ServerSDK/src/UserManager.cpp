/*
 * UserManager.cpp
 *
 *  Created on: Sep 14, 2017
 *      Author: eontorch
 */

#include "UserManager.h"

namespace std
{

UserManager::UserManager(FileManager *fm)
{
    fileManager = fm;
    listener = new MTCPListener();
}

UserManager::~UserManager()
{
}

MTCPListener *UserManager::getListener() const
{
    return listener;
}

void UserManager::setListener(MTCPListener *listener)
{
    this->listener = listener;
}

vector<User *> UserManager::getUsers()
{
    return users;
}

void UserManager::setUsers(vector<User *> users)
{
    this->users = users;
}

uMap *UserManager::getUsersMap()
{
    return fileManager->getUsersMap();
}

int UserManager::loginUser(string name, string pass)
{
    fileManager->readUsers(); //to update the map
    User *user = fileManager->getUserFromMap(name);
    if (user != NULL)
    {
        if (pass == user->getPass())
        {
            users.push_back(user);
            return 0;
        }
        else
        {
            cerr << "Wrong password!" << endl;
            return WRONG_PASS;
        }
    }
    else
    {
        cerr << "Didn't find the name: " << name << endl;
        return WRONG_USER;
    }
}

void UserManager::registerUser(string name, string pass)
{
    fileManager->readUsers();
    User *user = new User(name, pass);
    uMap::iterator iter;
    uMap *map = getUsersMap();
    iter = map->find(name);
    if (iter != map->end())
    {
        cerr << "The user already exists!" << endl;
    }
    else
    {
        fileManager->writeUser(user);
        uMap *map = fileManager->getUsersMap();
        uMap::iterator pos;
        if (map == NULL)
        {
            cerr << "MAP IS NULL" << endl;
        }
        else
        {
            cout << map->size() << endl;
        }
        for (pos = (*map).begin(); pos != (*map).end(); ++pos)
        {
            cout << "key: " << pos->first << " values: pass: " << pos->second.first << " score: " << pos->second.second << endl;
        }
    }
}

} /* namespace std */
