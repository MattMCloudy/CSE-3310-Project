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
    string c_top_temp = "chatroom";

    char* topic_name = strdup(c_top_temp.c_str());
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
    //start = clock();  
    //cout << "START TIME: " << start << endl; 
}


void Chatroom::sendChatroom() {
    //cout << chatroom_struct << "\n";
    ReturnCode_t status = Writer->write(*chatroom_struct, HANDLE_NIL);
    checkStatus(status, "Writer::write");
}

void Chatroom::addUser(User* new_user) {
    users.push_back(new_user);
}

void Chatroom::addMessage(Message* new_message) {
    messages.push_back(new_message);
    unpublished_messages.push_back(new_message);
    start = clock();  
}

void Chatroom::sendAllUnpublishedMessages() {
    for(int i = 0; i < unpublished_messages.size(); i++) {
        daemon->postNewMessageToUI(unpublished_messages[i]);
    }
    unpublished_messages.clear();
}

//same format as user setIsOnline
/*
    (megan) (4/22) (copy paste from Users.cpp. Same thing applies here)
    The elapsed_time check is just set to some arbitrary amout of time. 
    I messed with it forever trying to get it to be a determined amount 
    but it's all funky. We can either "eye-ball" it and set it to whatever 
    feels right. or someone can mess with it more if they want to make it check
    if like 5 min (or however much time we decided for someone to be inactive
    and declared offline) has passed. 
*/

void Chatroom::checkIfActive(){
    now_chatroom = clock(); 
    long double elapsed_time_chatroom; 
    elapsed_time_chatroom = double(now_chatroom - start_chatroom);
    if (elapsed_time_chatroom > 100000) 
        isActive = false;  
    else
        isActive = true;
}

//same format as user setIsOnline
void Chatroom::setIsActive() {
    start_chatroom = clock();
    checkIfActive();            //i don't think we need this line. double check. 
}

bool Chatroom::getIsActive(){
    return isActive; 
}

int Chatroom::getChatroomIndex() {return chatroom_idx;}

string Chatroom::getName() {return name;}


void Chatroom::changeName(string new_name) {
    name = new_name;
    strncpy(chatroom_struct->chatroom_name, new_name.c_str(), sizeof(chatroom_struct->chatroom_name));
    cout << "NEW CHATROOM NAME: " << chatroom_struct->chatroom_name << endl; 
}
struct chatroom* Chatroom::getChatroomStruct(){
    return chatroom_struct; 
}



