#include "../include/Message.h"

Message::Message(string text, unsigned long long passed_uuid, int passed_chatroom_idx) {
    content = text;
    chatroom_idx = passed_chatroom_idx;
    sender_uuid = passed_uuid;

    setEntityManager();
    makeNewMessage();
    sendMessage();   
}

Message::Message(struct message* new_mssg){
    string temp(new_mssg->message);
    content = temp;
    chatroom_idx = new_mssg->chatroom_idx;
    sender_uuid = new_mssg->uuid;

    message_struct = new_mssg;

}

Message::~Message() {
    em.deleteWriter();
    em.deletePublisher();
    em.deleteSubscriber();
    em.deleteTopic();
    em.deleteParticipant();
}

void Message::setEntityManager() {
    char* topic_name = "message";
    em.createParticipant("");

    messageTypeSupport_var T = new messageTypeSupport();
    em.registerType(T.in());
    em.createTopic(topic_name); // I thought this was better than passing in the topic name
    em.createPublisher();

    em.createWriter(false);
    DataWriter_var dwriter = em.getWriter();
    Writer = messageDataWriter::_narrow(dwriter.in());
    
    //Is there no check for writer?
    
    em.createSubscriber();
}

void Message::makeNewMessage() {
    struct message* new_message = (struct message*) malloc(sizeof(struct message)); //This is the same as messageInstance
    strncpy(new_message->message, content.c_str(), sizeof(new_message->message));
    new_message->uuid = sender_uuid;
    new_message->chatroom_idx = chatroom_idx;
    new_message->cksum = 0;
    message_struct = new_message;
}

unsigned long long Message::newBoostUUID() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    unsigned long long x;
    memcpy(&x, &uuid, sizeof(x));
    return x;
}

void Message::sendMessage() {
    ReturnCode_t status = Writer->write(*message_struct, HANDLE_NIL);
    checkStatus(status, "Writer::write");
}

int Message::getChatroomIndex() {return chatroom_idx;}

string Message::getContent() {return content;}

unsigned long long Message::getSenderUUID() {return sender_uuid;}





/*
void Message::makeTestMessage() {
    struct message* new_message = (struct message*) malloc(sizeof(struct message)); //This is the same as messageInstance
    string testMessage = "This is a test message"; 
    strncpy(new_message->message, testMessage.c_str(), sizeof(new_message->message));
    new_message->uuid = sender_uuid;
    new_message->chatroom_idx = chatroom_idx;
    new_message->cksum = 0;
    message_struct = new_message;
}
*/