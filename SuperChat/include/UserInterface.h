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
    public:
        UserInterface() {
            cout << "UI Initialized...\n";
        }
        void create();
        void setDaemon(ChatDaemon* new_daemon);
};

#endif
