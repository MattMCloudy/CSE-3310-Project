#include "../include/Message.h"

Message::Message(string text, long long passed_uuid, int passed_chatroom_idx) {
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

    message_struct = *new_mssg;

    setEntityManager();
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
    message new_mssg; //This is the same as messageInstance
    strncpy(new_mssg.message, content.c_str(), sizeof(new_mssg.message));
    new_mssg.uuid = newBoostUUID();
    new_mssg.chatroom_idx = chatroom_idx;
    new_mssg.cksum = 0;
    message_struct = new_mssg;
}

long long Message::newBoostUUID() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    long long x;
    memcpy(&x, &uuid, sizeof(x));
    return x;
}

void Message::sendMessage() {
    ReturnCode_t status = Writer->write(message_struct, HANDLE_NIL);
    checkStatus(status, "Writer::write");
}

int Message::getChatroomIndex() {return chatroom_idx;}

string Message::getContent() {return content;}
