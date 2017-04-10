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
        struct chatroom* chatroom_struct;
        string name;
        int chatroom_idx;
        DDSEntityManager em;
        chatroomDataWriter_var Writer;
        //chatroomDataWriter_var Reader;
        vector<User> users;
        vector<Message> messages;
    public:
        Chatroom(string passed_name, int passed_chatroom_idx,  ChatDaemon* passed_daemon);
        Chatroom(struct chatroom* passed_chatroom_struct, ChatDaemon* passed_daemon);
        ~Chatroom();
        void setEntityManager();
        void makeNewChatroom();
        void postMessage();
        void addUser(User* new_user);
        void sendMessage(Message* new_message);
        void recieveMessage(); //some params I need to figure out
        void sendChatroom();     
};

#endif
