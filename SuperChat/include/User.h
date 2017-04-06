#ifndef __USER_H
#define __USER_H

#include "Message.h"

using namespace DDS;
using namespace SuperChat;
using namespace std;

class User {
    private:
        string nick;
        unsigned long uuid;
        DDSEntityManager em;
        userDataWriter_var Writer;
        userDataReader_var Reader;
        //user_struct user;
    public:
        User(string nick);
        void createMessage();
        void sendMessage();
        void receiveMessage(); //I guess this is what we need here?
        //void ExitChatroom(); This will be in LocalUser
        //void EnterChatroom(); This will be too I think
};

#endif
