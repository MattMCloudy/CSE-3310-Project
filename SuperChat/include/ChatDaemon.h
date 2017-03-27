#ifndef __CHATDAEMON_H
#define __CHATDAEMON_H

#include "Chatroom.h"

class UserInterface;

using namespace std;

class ChatDaemon {
    private:
        UserInterface* ui;
    public:
        ChatDaemon() {
            cout << "Daemon Created...\n";
        }
        void start();
        void setUI(UserInterface* new_ui);
};

#endif
