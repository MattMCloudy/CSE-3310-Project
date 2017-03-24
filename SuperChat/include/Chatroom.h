#include "chatroom_c_struct.h"
#include "User.h"

#ifndef __CHATROOM_H
#define __CHATROOM_H

class Chatroom {
    private:
        //chatroom_struct chatroom;
        vector<User> users;
    public:
        void PostMessage();
        //void AddUser(); ??
        
};

#endif
