/*Matt:
 * Welcome to the home of the entire UserInterface for SuperChat.
 * Here is where will produce the ncurses gui, listen for user input, and
 * make requests to ChatDaemon as necessary for sending messages, adding users,
 * adding chatrooms, exiting the program, and anything else we can dream up.
 * Be careful to use mutex locks whenever you make any calls to ChatDaemon, as
 * it is highly probable it will break if we don't. Other than that, go wild.
 * Feel free to add any methods necessary to make it beautiful. Right now, I just
 * have the code for the hello world example inside of create.
 * GO TO UserInterface.cpp
 * or if you haven't already GO TO ChatDaemon.h
 */

#ifndef __USERINTERFACE_H
#define __USERINTERFACE_H

#include "Chatroom.h"
#define MAX_MESSAGE_LENGTH 144
#define C_MAX 121
#define R_MAX 40
#define CHATROOMS_NAME_MAX 25
#define MAX_UUID_LENGTH 10

class ChatDaemon;

using namespace std;

class UserInterface {
    private:
        ChatDaemon* daemon;
        mutex* m;
        int ch;
        char msg[MAX_MESSAGE_LENGTH];           // **THIS IS THE STRING YOU WANT TO GIVE ALL MESSAGES TO**
	string input;
        int i;
        int count;
        User* local_user;
	int function=0;
    public:
        UserInterface() {
            //cout << "UI Initialized...\n";
            count = 0;
        }
        void createChatroom();
        void create();
        void printMessage(User*, Message*, int);
        void setDaemon(ChatDaemon* new_daemon);
        void setMutex(mutex* new_m);
	void printChatrooms(vector<Chatroom*> chatrooms);
	void printUsers(vector<User*> online, vector<User*> offline);
 	void printError(string message);
};

#endif
