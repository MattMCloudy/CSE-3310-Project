#include "../include/User.h"


User::User(string passed_nick, string passed_description, int passed_chatroom_idx) {
    nick = passed_nick;
    description = passed_description;
    chatroom_idx = passed_chatroom_idx;
    uuid = newBoostUUID();

    setEntityManager();
    makeNewUser();
          
    sendUser();
}

User::User(struct user* new_user) {
    string nick_temp(new_user->nick);
    nick = nick_temp;
    chatroom_idx = new_user->chatroom_idx;
    //string desc_temp(new_user->description); the idl doesn't have this yet for some reason.
    //description = desc_temp;
    uuid = new_user->uuid;
    user_struct = new_user;

    setEntityManager();
    sendUser();
}


User::~User() {
    em.deleteWriter();
    em.deletePublisher();
    em.deleteSubscriber();
    em.deleteTopic();
    em.deleteParticipant();
}

void User::setEntityManager() {
    char* topic_name = "user";
    em.createParticipant("");

    userTypeSupport_var T = new userTypeSupport();
    em.registerType(T.in());
    em.createTopic(topic_name); // I thought this was better than passing in the topic name
    em.createPublisher();

    em.createWriter(false);
    DataWriter_var dwriter = em.getWriter();
    Writer = userDataWriter::_narrow(dwriter.in());
    
    //Is there no check for writer?
    
    em.createSubscriber();
}


void User::makeNewUser() {
    struct user new_user; //This is the same as messageInstance
    strncpy(new_user.nick, nick.c_str(), sizeof(new_user.nick));
    new_user.uuid = newBoostUUID();
    new_user.chatroom_idx = chatroom_idx;
    user_struct = &new_user;
}

long long int User::newBoostUUID() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    long long int x;
    memcpy(&x, &uuid, sizeof(x));
    return x;
}

void User::sendUser() {
    ReturnCode_t status = Writer->write(*user_struct, HANDLE_NIL);
    checkStatus(status, "Writer::write");
}

int User::getChatroomIndex() {return chatroom_idx;}

long long int User::getUUID() {return uuid;}

string User::getNick() {return nick;}
