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
