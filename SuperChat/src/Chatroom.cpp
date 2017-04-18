#include "../include/Chatroom.h"
#include "../include/ChatDaemon.h"


Chatroom::Chatroom(string passed_name, int passed_chatroom_idx, ChatDaemon* passed_daemon) {
    name = passed_name; /**/
    chatroom_idx = passed_chatroom_idx;
    daemon = passed_daemon;

    setEntityManager();
    makeNewChatroom();
    sendChatroom();
}

Chatroom::Chatroom(struct chatroom* new_chatroom, int new_chatroom_idx,  ChatDaemon* passed_daemon) {
    string name_temp(new_chatroom->chatroom_name);
    name = name_temp;
    new_chatroom->chatroom_idx = new_chatroom_idx;
    chatroom_idx = new_chatroom_idx;
    chatroom_struct = new_chatroom;
    
    daemon = passed_daemon;

}


Chatroom::~Chatroom() {
    em.deleteWriter();
    em.deletePublisher();
    em.deleteSubscriber();
    em.deleteTopic();
    em.deleteParticipant();
}

void Chatroom::setEntityManager() {
    char* topic_name = "chatroom";
    em.createParticipant("");

    chatroomTypeSupport_var T = new chatroomTypeSupport();
    em.registerType(T.in());
    em.createTopic(topic_name); // I thought this was better than passing in the topic name
    em.createPublisher();

    em.createWriter(false);
    DataWriter_var dwriter = em.getWriter();
    Writer = chatroomDataWriter::_narrow(dwriter.in());
    
    //Is there no check for writer?
    
    em.createSubscriber();
}


void Chatroom::makeNewChatroom() {
    struct chatroom* new_chatroom = (struct chatroom*) malloc(sizeof(struct chatroom));
    strncpy(new_chatroom->chatroom_name, name.c_str(), sizeof(new_chatroom->chatroom_name));
    new_chatroom->chatroom_idx = chatroom_idx;
    chatroom_struct = new_chatroom;
}


void Chatroom::sendChatroom() {
    ReturnCode_t status = Writer->write(*chatroom_struct, HANDLE_NIL);
    checkStatus(status, "Writer::write");
}

void Chatroom::addUser(User* new_user) {
    users.push_back(new_user);
}

void Chatroom::addMessage(Message* new_message) {
    messages.push_back(new_message);
    unpublished_messages.push_back(new_message);
}

void Chatroom::sendAllUnpublishedMessages() {
    for(int i = 0; i < unpublished_messages.size(); i++) {
        daemon->postNewMessageToUI(unpublished_messages[i]);
    }
    unpublished_messages.clear();
}

int Chatroom::getChatroomIndex() {return chatroom_idx;}

string Chatroom::getName() {return name;}
