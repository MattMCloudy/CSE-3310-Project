#ifndef __CHATROOM_H
#define __CHATROOM_H

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
        bool isActive; 
        //chatroomDataWriter_var Reader;
        vector<User*> users;
        vector<Message*> messages;
        vector<Message*> unpublished_messages;
        clock_t start_chatroom; 
        clock_t now_chatroom; 

    public:
        Chatroom(string passed_name, int passed_chatroom_idx,  ChatDaemon* passed_daemon);
        Chatroom(struct chatroom* passed_chatroom_struct, int new_chatroom_idx, ChatDaemon* passed_daemon);
        ~Chatroom();
        clock_t start; 
        
        void setEntityManager();
        void makeNewChatroom();
        void postMessage();
        void addUser(User* new_user);
        void addMessage(Message* new_message);
        void sendAllUnpublishedMessages();
        void recieveMessage(); //some params I need to figure out
        void sendChatroom();
        int getChatroomIndex(); 
        string getName();  
        void checkIfActive(); 
        void setIsActive();
        bool getIsActive(); 

        struct chatroom* getChatroomStruct(); 
        void changeName(string new_name);           //used to clear chatroom name after inactivity
};

#endif
