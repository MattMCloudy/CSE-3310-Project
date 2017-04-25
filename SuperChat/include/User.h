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
        string uuid_str;
        const char* uuid_char; 
        //clock_t start;
        clock_t now; 
        //time_t start; 
        //time_t now; 
        DDSEntityManager em;
        userDataWriter_var Writer;
    public:
        clock_t start;
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
        const char* getUUIDchar(); 
};

#endif
