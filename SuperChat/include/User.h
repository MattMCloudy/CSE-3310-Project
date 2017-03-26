#ifndef __USER_H
#define __USER_H

#include "user_c_struct.h"
#include "Message.h"

using namespace std;

class User {
    private:
        //user_struct user;
    public:
        void CreateMessage();
        void SendMessage();
        void ExitChatroom();
        //void EnterChatroom(); ??
};

#endif
