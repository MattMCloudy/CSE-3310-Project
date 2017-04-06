#ifndef __CHATROOM_H
#define __CHATROOM_H

#include "RemoteUser.h"
#include "LocalUser.h"
#include "User.h"

class ChatDaemon;

using namespace std;

class Chatroom {
    private:
        //chatroom_struct chatroom;
        vector<User> users;
    public:
        void PostMessage();
        //void AddUser(); ??
        
};

#endif
