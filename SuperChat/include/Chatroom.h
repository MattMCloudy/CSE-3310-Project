#ifndef __CHATROOM_H
#define __CHATROOM_H

#include "RemoteUser.h"
#include "LocalUser.h"
#include "User.h"

class ChatDaemon;

using namespace DDS;
using namespace SuperChat;
using namespace std;

class Chatroom {
    private:
        ChatDaemon* daemon;
        string name;
        DDSEntityManager em;
        chatroomDataWriter_var Writer;
        chatroomDataWriter_var Reader;
        vector<User> users;
    public:
        Chatroom(string name, ChatDaemon* daemon);
        void postMessage();
        void addUser(User* new_user);
        void sendMessage(Message* new_message);
        void recieveMessage(); //some params I need to figure out
        
};

#endif
