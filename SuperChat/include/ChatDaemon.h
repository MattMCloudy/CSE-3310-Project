/*
Matt:
 * OK finally we've found where the magic happens,
 * A daemon is a program that runs constantly underneath other user-level
 * programs, and I think it helps to think of this class in that way.
 * This will handle literally everything concerning the actual logic
 * of the program. Adding Chatrooms, Sending Messages, Adding Users, all of
 * these actions will have their origins in this class. However I'm not entirely sure
 * how all of that is going to work in reality. There is another class, ViewController
 * that we could enlist to be the middle man between this and UserInterface, but I think
 * we could also have the threads interweave between each other. For instance, while in
 * UserInterface the local user sends a message, then we could call a mutex lock there then
 * ui will make a method call to ChatDaemon to send that message. It sounds
 * a little hacky, but I think it might work. I'd love to know your thoughts.
 * So yeah, go crazy. Add methods here willy nilly and implement them in ChatDaemon.cpp.
 * As we build this thing up it will become more and more clear how we should design the
 * lower-level classes like Chatroom, User, and Message even though a lot of those decisions
 * have already been made for us.
 * GO TO ChatDaemon.cpp
 */

#ifndef __CHATDAEMON_H
#define __CHATDAEMON_H

#include "Chatroom.h"

class UserInterface;

using namespace std;

class ChatDaemon {
    private:
        UserInterface* ui;
        Chatroom* current_chatroom;
        bool LocalUserInitialized;
        bool hasStarted;
        vector<Chatroom*> chatrooms;
        vector<User*> users;
        vector<User*> online_users;
        vector<User*> offline_users;
        vector<Message*> messages;
        unordered_map<int, Chatroom*> chatroom_map;
        unordered_map<long long int, User*> user_map;
        unordered_map<int, Message*> message_map;
        User* local_user;
        DDSEntityManager chtrmEM;
        DDSEntityManager mssgEM;
        DDSEntityManager usrEM;
        chatroomDataReader_var chatroom_reader;
        messageDataReader_var message_reader;
        userDataReader_var user_reader;
        mutex* m;
    public:
        ChatDaemon() {
            cout << "ChatDaemon Initialized...\n";
            LocalUserInitialized = false;
            hasStarted = false;
        }
        ~ChatDaemon();
        void start();
        void setUI(UserInterface* new_ui);
        void setMutex(mutex* new_m);
        void setEntityManager();
        void setAllUsersOffline();
        vector<User*> checkWhichUsersOnline();
        vector<User*> getOfflineUsers();
        void readInAllUsers();
        void readInAllMessages();
        void readInAllChatrooms();
        Chatroom* createNewChatroom(string name);
        void postChatroomToUI(Chatroom* chatroom); //Possibly in Chatroom instead?
        void postNewMessageToUI(Message*); //This could also be inside of Chatroom theoretically.
        User* addNewLocalUser(string nick);
        User* addNewRemoteUser(User* new_user);
        void sendMessage(string text);
        void readSendObjects();
        void processCurrentChatroom();
        int hash(string key_string);
        void changeChatroom(Chatroom* new_cur_chatroom);
        void readInPreviousUsers();
        void postUsersToFile();
        void exit();      
};

#endif
