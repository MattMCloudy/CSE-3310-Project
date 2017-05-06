#include "../include/ChatDaemon.h"
#include "../include/UserInterface.h"

void ChatDaemon::start() {
    hasStarted = true;
    
    setEntityManager();
    readInAllChatrooms();

    if (chatrooms.size() == 0) {
        createNewChatroom("public");
    } else {
        changeChatroom(chatrooms[0]->getName());
    }
    readInAllChatrooms(); 
    changeChatroom(chatrooms[0]->getName());
    readSendObjects();
}

void ChatDaemon::setUI(UserInterface* new_ui) {
    ui = new_ui;
}

void ChatDaemon::setMutex(mutex* new_m) {
    m = new_m;
}


void ChatDaemon::setEntityManager() {
    string u_top_temp = "user";
    string m_top_temp = "message";
    string c_top_temp = "chatroom";
	

    char* user_topic_name = strdup(u_top_temp.c_str());
    char* message_topic_name = strdup(m_top_temp.c_str());
    char* chatroom_topic_name = strdup(c_top_temp.c_str());

    chtrmEM.createParticipant("");
    mssgEM.createParticipant("");
    usrEM.createParticipant("");

    userTypeSupport_var T_user = new userTypeSupport();
    usrEM.registerType(T_user.in());
    usrEM.createTopic(user_topic_name);

    messageTypeSupport_var T_message = new messageTypeSupport();
    mssgEM.registerType(T_message.in());
    mssgEM.createTopic(message_topic_name);

    chatroomTypeSupport_var T_chatroom = new chatroomTypeSupport();
    chtrmEM.registerType(T_chatroom.in());
    chtrmEM.createTopic(chatroom_topic_name);
    
    chtrmEM.createSubscriber();
    mssgEM.createSubscriber();
    usrEM.createSubscriber();

    usrEM.createReader();
    DataReader_var userreader = usrEM.getReader();

    mssgEM.createReader();
    DataReader_var mssgreader = mssgEM.getReader();

    chtrmEM.createReader();
    DataReader_var chtrmreader = chtrmEM.getReader();

    user_reader = userDataReader::_narrow(userreader.in());
    message_reader = messageDataReader::_narrow(mssgreader.in());
    chatroom_reader = chatroomDataReader::_narrow(chtrmreader.in());
    checkHandle(user_reader.in(), "MsgDataReader::_narrow");
    checkHandle(message_reader.in(), "MsgDataReader::_narrow");
    checkHandle(chatroom_reader.in(), "MsgDataReader::_narrow");
}

ChatDaemon::~ChatDaemon() {
    if (hasStarted) {
        chtrmEM.deleteReader();
        chtrmEM.deleteSubscriber();
        chtrmEM.deleteTopic();
        chtrmEM.deleteParticipant();

        usrEM.deleteReader();
        usrEM.deleteSubscriber();
        usrEM.deleteTopic();
        usrEM.deleteParticipant();

        mssgEM.deleteReader();
        mssgEM.deleteSubscriber();
        mssgEM.deleteTopic();
        mssgEM.deleteParticipant();
    }  
}


vector<User*> ChatDaemon::checkWhichUsersOnline() {
    online_users.clear();
    offline_users.clear();
    for(int i = 0; i < users.size(); i++) {
        users[i]->checkIfOnline();
        if (users[i]->getIsOnline()){
            online_users.push_back(users[i]); 
        }
        else{
            offline_users.push_back(users[i]);
        }
    }
    return online_users;
}

vector<User*> ChatDaemon::getOfflineUsers() {return offline_users;}

vector<Chatroom*> ChatDaemon::checkWhichChatroomsActive() {
    active_chatrooms.clear();
    for(int i = 0; i < chatrooms.size(); i++) {
        chatrooms[i]->checkIfActive();
        if (chatrooms[i]->getIsActive()){
            active_chatrooms.push_back(chatrooms[i]);
        }
    }
    return active_chatrooms;
}



void ChatDaemon::readInAllUsers() {
    userSeq userList;
    SampleInfoSeq infoSeq;
    ReturnCode_t status = -1;
    
    status = user_reader->take(userList, infoSeq, LENGTH_UNLIMITED, ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
    checkStatus(status, "MsgDataReader::take");
    
    for(ULong j = 0; j < userList.length(); j++) {
        User* new_user = new User(&userList[j]);

        if (user_map[new_user->getUUID()] != NULL) {
            user_map[new_user->getUUID()]->setIsOnline();
            continue;
        }
        users.push_back(new_user);
        user_map[new_user->getUUID()] = new_user;
        chatrooms[new_user->getChatroomIndex()]->addUser(new_user);  
        postUsersToUI();
    }
    
    checkWhichUsersOnline();
    postUsersToUI();

    status = user_reader->return_loan(userList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}



void ChatDaemon::readInAllMessages() {
    messageSeq msgList;
    SampleInfoSeq infoSeq;
    ReturnCode_t status = -1;
    status = message_reader->take(msgList, infoSeq, LENGTH_UNLIMITED, ANY_SAMPLE_STATE,
                            ANY_VIEW_STATE, ANY_INSTANCE_STATE);
    checkStatus(status, "MsgDataReader::take");
    
    for(ULong j = 0; j < msgList.length(); j++) {
        Message* new_message = new Message(&msgList[j]);
        messages.push_back(new_message);
        message_map[hash(new_message->getContent())] = new_message;
	m->lock();
        chatrooms[new_message->getChatroomIndex()]->addMessage(new_message);
        chatrooms[new_message->getChatroomIndex()]->setIsActive();
	m->unlock();
    }

    status = message_reader->return_loan(msgList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}

void ChatDaemon::readInAllChatrooms() {
    chatroomSeq chatList;
    SampleInfoSeq infoSeq;
    ReturnCode_t status = -1;
    status = chatroom_reader->take(chatList, infoSeq, LENGTH_UNLIMITED, ANY_SAMPLE_STATE,
                            ANY_VIEW_STATE, ANY_INSTANCE_STATE);
    checkStatus(status, "MsgDataReader::take");
    
    for(ULong j = 0; j < chatList.length(); j++) {
        Chatroom* new_chatroom = new Chatroom(&chatList[j], chatrooms.size(), this);
        if (chatrooms.size() >= 10)
            continue;
        else if (chatroom_map[hash(new_chatroom->getName())] != NULL) 
            continue;              
        else{
            chatroom_map[hash(new_chatroom->getName())] = new_chatroom;
            chatrooms.push_back(new_chatroom);
            if (current_chatroom == NULL){
                changeChatroom(new_chatroom->getName());  
            } 
	    postChatroomsToUI();    
        }
    }
    checkWhichChatroomsActive(); 
    status = chatroom_reader->return_loan(chatList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}

User* ChatDaemon::changeLocalUserNick(string input) {
    local_user->changeNick(input);
    return local_user;
}

Chatroom* ChatDaemon::changeChatroomName(string input) {
    current_chatroom->changeName(input);
    return current_chatroom;
}


Chatroom* ChatDaemon::createNewChatroom(string name) {                  
    if (chatrooms.size() >= 10) {
        //cerr << "ERROR: Already 10 chatrooms initialized";
    } else {
        Chatroom* new_chatroom = new Chatroom(name, chatrooms.size(), this);
        new_chatroom->setIsActive();
        chatrooms.push_back(new_chatroom);
        local_chatrooms.push_back(new_chatroom);
        chatroom_map[hash(new_chatroom->getName())] = new_chatroom;
        changeChatroom(new_chatroom->getName());
	checkWhichChatroomsActive();
        postChatroomsToUI();
        return new_chatroom;
    }
    return NULL;
}

void ChatDaemon::processCurrentChatroom() {
    m->lock();
    current_chatroom->sendAllUnpublishedMessages();
    m->unlock();
}

void ChatDaemon::postUsersToUI() {
    m->lock();
    ui->printUsers(online_users, offline_users);
    m->unlock();
    
}

void ChatDaemon::postChatroomsToUI() {
    m->lock();
    ui->printChatrooms(chatrooms);
    m->unlock();
}

void ChatDaemon::postNewMessageToUI(Message* new_message) {
    User* sender = user_map[new_message->getSenderUUID()];   
    
    ui->printMessage(sender, new_message, message_length_counter);
}   

User* ChatDaemon::addNewLocalUser(string nick) {
    if (LocalUserInitialized) {
        cerr << "local user initialized";
        return local_user;
    }
    
    User* new_local_user = new User(nick, "The local user", 0);
    local_user = new_local_user;
    user_map[local_user->getUUID()] = local_user;
    LocalUserInitialized = true;
    return local_user;
    
}

User* ChatDaemon::getLocalUser(){
    return local_user; 
}

void ChatDaemon::wakeLocalUser() {
    local_user->sendUser();
}

void ChatDaemon::readSendObjects() {
    while(true) {
        readInAllChatrooms();
        readInAllUsers();
        readInAllMessages();
        processCurrentChatroom();
        wakeLocalUser();                              
    }
}

void ChatDaemon::changeChatroom(string input) {
    m->lock();
    Chatroom* new_cur_chatroom = chatroom_map[hash(input)];	
    if (new_cur_chatroom == NULL) {
	recentErrorStatus = true;
	ui->printError("Attempted to enter a chatroom that does not exist");
	return;
    }
    current_chatroom = new_cur_chatroom;
    m->unlock();
}

void ChatDaemon::sendMessage(char* input) {
    string text(input);
    current_chatroom->setIsActive();
    Message* new_message = new Message(text, local_user->getUUID(), current_chatroom->getChatroomIndex());
}

void ChatDaemon::setTestMessage() {
    testMessage = "This is a test message."; 
}

void ChatDaemon::setMessageLengthCounter(int size) {
    message_length_counter = size;
}

int ChatDaemon::hash(string key_string) {
    int total = 0;
    
    for(int i = 0; i < key_string.length(); i++)
        total = total + (int) key_string[i];
    
    
    return total;
}

vector<Chatroom*> ChatDaemon::getChatrooms() {return chatrooms;}

void ChatDaemon::readInPreviousUsers() {
    /*some reading in files from cout and stuff */
}

bool ChatDaemon::isError() {
    bool temp = recentErrorStatus;
    recentErrorStatus = false;
    return temp;
}

void ChatDaemon::setChatbox(FORM* passed_chatbox) {chatbox = passed_chatbox;}


Chatroom* ChatDaemon::getCurrentChatroom(){
    return current_chatroom; 
}


    
