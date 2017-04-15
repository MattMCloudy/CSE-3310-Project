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

class ChatDaemon;

using namespace std;

class UserInterface {
    private:
        ChatDaemon* daemon;
        mutex* m;

        int MAX_MESSAGE_LENGTH; //MAX LENGTH FOR ENTIRE OUTPUT (INCLUDES UUID AND NICK)
        int R_MAX;
        int C_MAX;
        int MAX_UUID_LENGTH;
    public:
        UserInterface() {
            //cout << "UI Initialized...\n";
            MAX_MESSAGE_LENGTH = 144;
            R_MAX = 40;
            C_MAX = 121;
            MAX_UUID_LENGTH = 10;
        }
        void create();
        void printMessage(char*, char*, char*, FORM*, int);
        void setDaemon(ChatDaemon* new_daemon);
        void setMutex(mutex* new_m);
};

#endif
