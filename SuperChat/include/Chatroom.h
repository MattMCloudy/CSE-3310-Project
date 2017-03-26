#ifndef __CHATROOM_H
#define __CHATROOM_H

#include "chatroom_c_struct.h"
#include "User.h"

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
