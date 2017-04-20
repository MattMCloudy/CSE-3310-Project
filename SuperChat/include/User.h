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
        bool isOnline;
        int chatroom_idx;
        struct user* user_struct;
        unsigned long long uuid;
        clock_t start;
        DDSEntityManager em;
        userDataWriter_var Writer;
    public:
        User(string, string, int);
        User(struct user*);
        ~User();
        void setEntityManager();
        void makeNewUser();
        unsigned long long newBoostUUID();
        void sendUser();
        int getChatroomIndex();
        unsigned long long getUUID();
        string getNick();
        void changeNick(string);
        void checkIfOnline();
        void setIsOnline();
        bool getIsOnline();
};

#endif
