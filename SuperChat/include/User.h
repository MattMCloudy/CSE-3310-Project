#ifndef __USER_H
#define __USER_H

#include "Message.h"

using namespace DDS;
using namespace SuperChat;
using namespace std;

class User {
    private:
        string nick;
        string description;
        int chatroom_idx;
        struct user* user_struct;
        long long int uuid;
        DDSEntityManager em;
        userDataWriter_var Writer;
    public:
        User(string, string, int);
        User(struct user*);
        ~User();
        void setEntityManager();
        void makeNewUser();
        long long int newBoostUUID();
        void sendUser();
        int getChatroomIndex();
        long long int getUUID();   
};

#endif
