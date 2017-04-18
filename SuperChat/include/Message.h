//This should just be the cstruct that we talked about in class.
//If anyone wants this to be a class it can be, but we don't necessarily have to

#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <sstream>
#include <unordered_map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "SuperChat.h"
#include "DDSEntityManager.h"
#include "../bin/ccpp_SuperChat.h"

using namespace DDS;
using namespace SuperChat;
using namespace std;

/*
 * I don't believe we need a reader inside of Message,
 * Messages should be read in from Chatroom
 */

class Message {
    private:
        struct message* message_struct;
        DDSEntityManager em;
        messageDataWriter_var Writer;
        string content;
        unsigned long long sender_uuid;
        int chatroom_idx;      
    public:
        Message(string, unsigned long long, int);
        Message(struct message*);
        ~Message();
        void setEntityManager();
        void makeNewMessage();
        unsigned long long newBoostUUID();
        void sendMessage();
        int getChatroomIndex();
        string getContent();
        unsigned long long getSenderUUID();
};

#endif
