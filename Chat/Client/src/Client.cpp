#include <iostream>
#include "Client.h"
#include "ClientHandler.h"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void cliLogin(string user, string pass, Client *client)
{
    cout << "Enter your username:" << endl;
    cin >> user;
    cout << "Enter your password:" << endl;
    cin >> pass;
    client->login(user, pass);
}

void cliRegister(string user, string pass, Client *client)
{
    cout << "Enter your username:" << endl;
    cin >> user;
    cout << "Enter your password:" << endl;
    cin >> pass;
    client->registerUser(user, pass);
}

void cliShowUsers(Client *client)
{
    //send the server a command to show all users
    int cmdNet = htonl(SHOW_USERS);
    client->getSocket()->write((char *)&cmdNet, 4);

    sleep(3);

    cout << "The connected users are: " << endl;
    cout << client->getUserList() << endl;
}

void gameFunc(Client *client)
{

    int guess;
    int number;

    cout << "Welcome. Game started! All numbers should be between 1 and 10." << endl;
    do
    {
        cout << "Please choose your number for the other player to guess: ";
        cin >> number;
    } while (number < 1 || number > 10);
    client->setNumber(number);
    do
    {
        cout << "Please guess a number: ";
        cin >> guess;
    } while (guess < 1 || guess > 10);
    client->setGuess(guess);
    cout << "Game finished! Thanks for playing!" << endl;
}

int main()
{
    ClientHandler *handler = new ClientHandler();
    Client *client = new Client(handler);
    cout << "Hello, welcome to the guessing game!" << endl;
    bool connected = client->connect(IP);
    if (!connected)
    {
        do
        {
            cout << "Couldn't connect, trying again..." << endl;
            connected = client->connect(IP);
        } while (!connected);
    }
    cout << "You are now connected to the server." << endl;
    int choice;
    string user, pass, feedback, temp;
    stringstream strs, strs1;
    do
    {
        cout << "To register, press 1, to login, press 2." << endl;
        sleep(1);
        cin >> choice;
        if (choice == 2)
        {
            cliLogin(user, pass, client);
            sleep(1);
            feedback = client->getHandler()->getMSG();
            strs << WRONG_USER;
            temp = strs.str();

            if (feedback == temp)
            {
                cout << "Didn't find the username in the database!" << endl;
            }
            else
            {
                strs1 << WRONG_PASS;
                temp = strs1.str();
                if (feedback == temp)
                {
                    cout << "Wrong password!" << endl;
                }
            }
            temp = "LOGIN SUCCESS";
            if (feedback == temp)
            {
                //Client has logged in successfully
                cout << "Login success" << endl;
                client->setLogged(true);
                sleep(1);
            }
        }
        else if (choice == 1)
        {
            cliRegister(user, pass, client);
            cout << "Registered successfully!" << endl;
            cout << "Now please login:" << endl;
            cliLogin(user, pass, client);
            sleep(1);
            feedback = client->getHandler()->getMSG();
            strs << WRONG_USER;
            temp = strs.str();
            if (feedback == temp)
            {
                cout << "Didn't find the username in the database!" << endl;
            }
            else
            {
                strs1 << WRONG_PASS;
                temp = strs1.str();
                if (feedback == temp)
                {
                    cout << "Wrong password!" << endl;
                }
            }
            temp = "LOGIN SUCCESS";
            if (feedback == temp)
            {
                //Client has logged in successfully
                cout << "Login success" << endl;
                client->setLogged(true);
                sleep(1);
            }
        }
    } while ((choice != 1 || choice != 2) && !client->isLogged());

    while (connected)
    {
        while (client->isLogged())
        {
            int answer;
            //check if got a match request
            if (client->getHandler()->isInvited())
            {
                sleep(1);
                string msg = client->getHandler()->getMSG();
                do
                {
                    cout << client->getHandler()->getMSG() << endl;
                    cin >> answer;
                    if (answer == 6)
                    {
                        client->acceptGame();
                        gameFunc(client);
                        client->startTheGame();
                    }
                    else if (answer == 7)
                    {
                        client->declineGame();
                    }
                } while (answer != 6 && answer != 7);
                client->getHandler()->setInvited(false);
            }
            //end of match request check
            else
            {
                //				//check if didn't get a match request
                int command;
                bool sentGameReq;
                string opponent;
                string users;
                cout << "If you want to see available users, press " << SHOW_USERS << endl;
                cout << "If you want to open a game with another player, press " << OPEN_MATCH_WITH_USER << endl;
                cout << "If you want to open a game randomly, press " << OPEN_MATCH_RANDOM << endl;
                cin >> command;
                //check if didn't get a match request while waiting for input
                if (!client->getHandler()->isInvited())
                {
                    if (command == OPEN_MATCH_WITH_USER)
                    {
                        cout << "Please enter the name of the other player:" << endl;
                        cin >> opponent;
                    }
                    string acceptStr, declineStr;
                    switch (command)
                    {
                    case SHOW_USERS:
                        client->displayUsers();
                        sleep(2);
                        users = client->getHandler()->getMSG();
                        cout << users << endl;
                        break;
                    case OPEN_MATCH_RANDOM:
                        sentGameReq = client->openGameRandom();
                        if (!sentGameReq)
                        {
                            cerr << "Can't send game request!" << endl;
                        }
                        sleep(3);
                        acceptStr = "Opponent has accepted the challenge!";
                        declineStr = "Your opponent has refused to play with you. Please choose again.";
                        if (client->getHandler()->getMSG() == acceptStr)
                        {
                            cout << client->getHandler()->getMSG();
                        }
                        else if (client->getHandler()->getMSG() == declineStr)
                        {
                            cout << client->getHandler()->getMSG();
                        }
                        break;
                    case OPEN_MATCH_WITH_USER:
                        client->openGame(opponent);
                        acceptStr = "Opponent has accepted the challenge!";
                        declineStr = "Your opponent has refused to play with you. Please choose again.";
                        while (client->getHandler()->getMSG() != acceptStr && client->getHandler()->getMSG() != declineStr)
                        {
                            sleep(3);
                        }
                        if (client->getHandler()->getMSG() == acceptStr)
                        {
                            cout << "accepted" << endl;
                            gameFunc(client);
                            sleep(1);
                            if (client->getHandler()->getMSG() != acceptStr)
                                cout << "Score Updated!" << endl;
                        }
                        else if (client->getHandler()->getMSG() == declineStr)
                        {
                            cout << "declined" << endl;
                        }
                        break;
                    }
                }
                else
                {
                    cout << "An invitation just got received from another player!!!" << endl;
                }
            }
        }
        //user has chosen 3/4/5
    }
    return 0;
}
