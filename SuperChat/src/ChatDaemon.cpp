#include "../include/ChatDaemon.h"
#include "../include/UserInterface.h"



void ChatDaemon::start() {
    hasStarted = true;
    
    setEntityManager();
    readInAllChatrooms();

    if (chatrooms.size() == 0) {
        //cout << "Initializing public chatroom...\n";
        createNewChatroom("public");
    } else {
        //cout << "Public previously initialized, entering now...\n";
        changeChatroom(chatrooms[0]);
    }

    /*(megan) Initializing some online users to see if they show up 
      (4/22)  on the GUI online users list. 
    */

    User("Bobby", "testUser1", 0);   //assigns nick and chatroom_idx. Passes to makeNewUser which creates a user object. 
    readInAllUsers();
    User("Sam", "testUser2", 0);     //then publishes the new user. 
    readInAllUsers();
    User("Marthak", "testUser2", 0);  //isOnline set to true.
    readInAllUsers();
    User("TommyII", "testUser2", 0);
    readInAllUsers();
    User("Sarah", "testUser3", 0); //BUG: doesn't display the last user to be read in. 
    readInAllUsers(); 


    
    readSendObjects();
 }

void ChatDaemon::setUI(UserInterface* new_ui) {
    ui = new_ui;
}

void ChatDaemon::setMutex(mutex* new_m) {
    m = new_m;
}


void ChatDaemon::setEntityManager() {
    char* user_topic_name = "user";
    char* message_topic_name = "message";
    char* chatroom_topic_name = "chatroom";

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
            //cout << "Online USER NAME:  " << users[i]->getNick() << endl; 
            //cout << "Online USER START: " << users[i]->start << endl; 
            //cout << "ONLINE USER: " << online_users[i]->getNick() << endl; 
        }
        else{
            //cout << "Offline USER NAME:  " << users[i]->getNick() << endl; 
            //cout << "Offline USER START: " << users[i]->start << endl;
            offline_users.push_back(users[i]);
            //cout << "OFFLINE USER in c.w.u.o: " << offline_users[i]->getNick() << endl; 
        }
    }
    return online_users;
}

vector<User*> ChatDaemon::getOfflineUsers() {return offline_users;}

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
        
        /*
        cout << "New User to be added...\n";
        cout << "Nick: " << new_user->getNick() << "\n";
        cout << "UUID: " << new_user->getUUID() << "\n";
        cout << "Index: " << new_user->getChatroomIndex() << "\n";
        cout << "Chatroom ptr: " << chatrooms[new_user->getChatroomIndex()] << "\n";
        */

        users.push_back(new_user);
        user_map[new_user->getUUID()] = new_user;
        chatrooms[new_user->getChatroomIndex()]->addUser(new_user);  
        postUsersToUI();        //sends vector<Users*>online_users and vector<User*>offline_users
                                //to UI to be printed. 
    }
    
    checkWhichUsersOnline();    //sorts the offline and online ones into appropriate vectors. 
                                //returns vector<Users*>online_users; 
    postUsersToUI();            //megan added. needed in order to update lists so they move from 
                                //online in GUI to offline in GUI
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

        /*
        cout << "Message Sender: " << new_message->getSenderUUID() << "\n";
        cout << "Message Text: " << new_message->getContent() << "\n";
        cout << "Message Chatroom Index: " << new_message->getChatroomIndex() << "\n";
        */

        messages.push_back(new_message);
        message_map[hash(new_message->getContent())] = new_message;
        chatrooms[new_message->getChatroomIndex()]->addMessage(new_message);
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
            new_chatroom->setIsActive(); 
        else{
            chatroom_map[hash(new_chatroom->getName())] = new_chatroom;
            chatrooms.push_back(new_chatroom);
            
            if (current_chatroom == NULL)
                changeChatroom(new_chatroom);   

            new_chatroom->setIsActive();
            postChatroomsToUI();
        }
    }

    status = chatroom_reader->return_loan(chatList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}

User* ChatDaemon::changeLocalUserNick(char* input) {
    string temp(input);
    local_user->changeNick(temp);
    return local_user;
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
        changeChatroom(new_chatroom);
        postChatroomsToUI();
        return new_chatroom;
    }
    return NULL;
}

void ChatDaemon::processCurrentChatroom() {
    current_chatroom->sendAllUnpublishedMessages();
}

void ChatDaemon::postUsersToUI() {
    m->lock();
    ui->printUsers(online_users, offline_users);
    m->unlock();
    
}

void ChatDaemon::postChatroomsToUI() {
    //Something like this should work for this method
    
    
    m->lock();
    ui->printChatrooms(chatrooms);
    m->unlock();
    
    
}

void ChatDaemon::postNewMessageToUI(Message* new_message) {
    User* sender = user_map[new_message->getSenderUUID()];   
    
    m->lock();
    ui->printMessage(sender, new_message, message_length_counter);
    m->unlock();
}   

User* ChatDaemon::addNewLocalUser(string nick) {    //sent the inputted name from GUI
   // cout << "TESTING IN ADD NEW LOCAL USER" << endl; 
    if (LocalUserInitialized) {
        //cerr << "A local user has already been initialized: ";
        return local_user;
    }
    
    User* new_local_user = new User(nick, "The local user", 0);    //during construction, the nick becomes a User (given uuid and chatroom idx)
    local_user = new_local_user;                                   //and the constructor also send the nick to makeNewUser() where it is  
    user_map[local_user->getUUID()] = local_user;                  //actually given a user struct for idl 
    LocalUserInitialized = true;
    return local_user;
    
}
User* ChatDaemon::getLocalUser(){
    return local_user; 
}

void ChatDaemon::wakeLocalUser() {local_user->sendUser();}

void ChatDaemon::readSendObjects() {
    while(true) {
        readInAllChatrooms();
        readInAllUsers();
        readInAllMessages();
        processCurrentChatroom();
       
       // wakeLocalUser();
        sleep(3); 
    }
}

void ChatDaemon::changeChatroom(Chatroom* new_cur_chatroom) {
    current_chatroom = new_cur_chatroom;
}

void ChatDaemon::sendMessage(char* input) {
    string text(input);
    current_chatroom->setIsActive();
    Message* new_message = new Message(text, local_user->getUUID(), current_chatroom->getChatroomIndex());
}

void ChatDaemon::setTestMessage(){
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


void ChatDaemon::setChatbox(FORM* passed_chatbox) {chatbox = passed_chatbox;}


