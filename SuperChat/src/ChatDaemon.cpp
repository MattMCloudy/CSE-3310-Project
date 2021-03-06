#include "../include/ChatDaemon.h"
#include "../include/UserInterface.h"

/*
(megan, 4/23) Bugs I know of:   I think the last user read in doesn't get displayed 
                                When user creates new chatroom and names it, the buffer string still contains old message so it gets put on the name. 
                                The public chatroom also disappears after inactivite.     
                                Lots of other noticable ones I'm sure. But I like our programs. :) 
*/

void ChatDaemon::start() {
    hasStarted = true;
    
    setEntityManager();
    readInAllChatrooms();

    if (chatrooms.size() == 0) {
        //cout << "Initializing public chatroom...\n";
        createNewChatroom("public");
    } else {
        //cout << "Public previously initialized, entering now...\n";
        changeChatroom(chatrooms[0]->getName());
    }
    //createNewChatroom("Test room A "); 
    //createNewChatroom("Test room B "); 
    readInAllChatrooms(); 
    changeChatroom(chatrooms[0]->getName());

    /*(megan) Initializing some online users to see if they show up 
      (4/22)  on the GUI online users list. 
    */
    
    /*
    User("Bobby", "testUser1", 0);      //assigns nick and chatroom_idx. Passes to makeNewUser which creates a user object. 
    readInAllUsers();                   //then publishes the new user.
    User("Sam", "testUser2", 0);        //isOnline set to true.
    readInAllUsers();
    User("Marthak", "testUser2", 2);    
    readInAllUsers();                      
    User("TommyII", "testUser2", 0);
    readInAllUsers();
    User("Sarah", "testUser3", 0);
    */
    //BUG: doesn't display the last user to be read in. 
    
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
        chatrooms[new_message->getChatroomIndex()]->setIsActive();                //start_chatroom is reset to this moment. 
    }

    status = message_reader->return_loan(msgList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}

/*Function: readInAllChatrooms: gets all the published chatrooms from the network.
                instances a new Chatroom() during which an idl chatroom struct is
                also instanced. 
                if there are already 10 chatrooms, it doesn't do anything with the
                new room. 
                if the chatroom is already in the hash table, (based on name) it doesn't do anything
                with the room. 
                otherwise, it hashes the new chatroom into the table, adds the chatroom to the 
                vector<Chatroom*>chatrooms. 
                If the current_chatroom isn't set, we go to the new chatroom. 
*/


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

/*
Function:    createNewChatroom
Parameters:  name - a string representing the neme of the chatroom to be created
             It is the name that the user inputs when creating a new chatroom from
             GUI.
Returns:     Chatroom* - a pointer to a new Chatroom instance. 
Description: When the user enters the name he wants for his new
             chatroom, it is sent here from UserInterface. 
             1: Constructor makes new instance of User
                a) constructor also makes the new chatroom into an idl chatroom struct. 
                b) constructor publishes new chatroom. 
             2: This new chatroom is added to vector<Chatrooms*>chatrooms. 
             3: Added to local_chatrooms (?)
             4: Current chatroom is updated to new chatroom. 
             5: Sent to UI for printing. 
*/
Chatroom* ChatDaemon::createNewChatroom(string name) {                  
    if (chatrooms.size() >= 10) {
        //cerr << "ERROR: Already 10 chatrooms initialized";
    } else {
        Chatroom* new_chatroom = new Chatroom(name, chatrooms.size(), this);
        new_chatroom->setIsActive();
        chatrooms.push_back(new_chatroom);
        local_chatrooms.push_back(new_chatroom);
       // cout << "NEW CHATROOM NAME: " << new_chatroom->getName() << endl; 
        chatroom_map[hash(new_chatroom->getName())] = new_chatroom;
        changeChatroom(new_chatroom->getName());
	checkWhichChatroomsActive();
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

User* ChatDaemon::addNewLocalUser(string nick) {                   //sent the inputted name from GUI
    if (LocalUserInitialized) {
        cerr << "local user initialized";
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
        wakeLocalUser();                              
    }
}

void ChatDaemon::changeChatroom(string input) {
    Chatroom* new_cur_chatroom = chatroom_map[hash(input)];	
    if (new_cur_chatroom == NULL) {
	recentErrorStatus = true;
	ui->printError("Attempted to enter a chatroom that does not exist");
	return;
    }
    current_chatroom = new_cur_chatroom;
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


    
