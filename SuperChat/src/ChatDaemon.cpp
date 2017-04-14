/*
Matt:
 * OK, this is where all of the methods in ChatDaemon will be implemented.
 * I bet we'll have to get pretty cozy with this file.
 * Thanks for following along on this little tour, lmk any questions/concerns you
 * may have.
 * <3 you groupies.
 * Matt out.
 */


#include "../include/ChatDaemon.h"
#include "../include/UserInterface.h"


void ChatDaemon::start() {
    hasStarted = true;

    //m->lock();
    //build user list from file ** needs to be written
    //readInPreviousUsers();
    //m->unlock();
    
    //Update user list with DDS
    //Update message list with DDS
    //Update chatroom list with DDS
    setEntityManager();

    readInAllChatrooms();
    readInAllUsers();
    readInAllMessages();
    
    //once we read in All Users too, we'll be able
    //to designate which ones are online and which
    //ones are not as well.
    

    //Then if no chatrooms have been initialized
    //create public

    //otherwise we need it to enter chatroom 0, which should be public
    
    //by this time the ui should have sent a command to initialize the
    //Local User which upon being created will send itself out to the world
    //this thread of execution will be locked to make all of that happen
    
    //from there it should conduct normal operating procedure
    //that is periodically updating the user list, message list, and chatroom list
    //assigning users to their appropriate chatroom, assigning messages to their appropriate chatroom,
    //and taking in new chatrooms

    //from there it will look at the information stored inside of the current chatroom
    //it will need to keep three vectors. One containing all of its associated users, and
    //two containing posted messages and unposted messages.
    
    //we just need to post all the unposted messages, clear that vector, and do all that all over again
    
    //I need to think more about how this will affect threading

    if (chatrooms.size() == 0) {
        cout << "Initializing public chatroom...\n";
        createNewChatroom("public");
    } else {
        cout << "Public previously initialized, entering now...\n";
        changeChatroom(chatrooms[0]);
    }
    
    addNewLocalUser("Tim");
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

void ChatDaemon::setAllUsersOffline() {
    for(int i = 0; i < users.size(); i++) {
        users[i]->setIsOffline();
    }
}

vector<User*> ChatDaemon::checkWhichUsersOnline() {
    online_users.clear();
    offline_users.clear();
    for(int i = 0; i < users.size(); i++) {
        if (users[i]->getIsOnline())
            online_users.push_back(users[i]);
        else
            offline_users.push_back(users[i]);
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
    
    m->lock();
    setAllUsersOffline();
    m->unlock();

    for(ULong j = 0; j < userList.length(); j++) {
        User* new_user = new User(&userList[j]);

        if (user_map[new_user->getUUID()] != NULL) {
            user_map[new_user->getUUID()]->setIsOnline();
            continue;
        }
        

        cout << "New User to be added...\n";
        cout << "Nick: " << new_user->getNick() << "\n";
        cout << "Index: " << new_user->getChatroomIndex() << "\n";
        cout << "Chatroom ptr: " << chatrooms[new_user->getChatroomIndex()] << "\n";
        users.push_back(new_user);
        user_map[new_user->getUUID()] = new_user;
        chatrooms[new_user->getChatroomIndex()]->addUser(new_user);
    }
    
    
    checkWhichUsersOnline();
    //We can have this written here or in the GUI
    //honestly I'd prefer it be in the GUI
    //it'll make the whole thing less computationally intensive

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

        if(message_map[hash(new_message->getContent())] != NULL)
            continue;

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
        //ffr we should probably throw some type of check in here
        //you see we read in any chatrooms we create, and
        //this will recreate them as it stands now
        //not good lol
        

        Chatroom* new_chatroom = new Chatroom(&chatList[j], chatrooms.size(), this);

        if ((chatrooms.size() >= 10) || (chatroom_map[hash(new_chatroom->getName())] != NULL))
            continue;
        
        chatroom_map[hash(new_chatroom->getName())] = new_chatroom;
        chatrooms.push_back(new_chatroom);
    }

    status = chatroom_reader->return_loan(chatList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}


Chatroom* ChatDaemon::createNewChatroom(string name) {
    //Trap from UI to here to create a new Chatroom 
    //possibly need more than name for params
    
    //Something like this should work for making chatrooms

    if (chatrooms.size() >= 10) {
        cerr << "ERROR: Already 10 chatrooms initialized";
    } else {
        Chatroom* new_chatroom = new Chatroom(name, chatrooms.size(), this);
        chatroom_map[hash(name)] = new_chatroom;
        chatrooms.push_back(new_chatroom);
        changeChatroom(new_chatroom);
        postChatroomToUI(new_chatroom);
        return new_chatroom;
    }

    return NULL;
}

void ChatDaemon::processCurrentChatroom() {
    current_chatroom->sendAllUnpublishedMessages();
}

void ChatDaemon::postChatroomToUI(Chatroom* chatroom) {
    //Something like this should work for this method
    
    /*
    m->lock();
    ui->postNewChatroom(chatroom);
    m->unlock();
    */
    
}

void ChatDaemon::postNewMessageToUI(Message* new_message) {
   
    /*
    m->lock();
    ui->postNewMessage(message);
    m->unlock();
    */

}

User* ChatDaemon::addNewLocalUser(string nick) {
    
    if (LocalUserInitialized) {
        cerr << "A local user has already been initialized: ";
        return local_user;
    }
    
    cout << "Initializing Local User...\n";
    User* new_local_user = new User(nick, "The local user", 0);
    user_map[new_local_user->getUUID()] = new_local_user;
    users.push_back(new_local_user);
    local_user = new_local_user;
    
    
    LocalUserInitialized = true;
    return local_user;
    
}


void ChatDaemon::readSendObjects() {
    while(true) {
        readInAllChatrooms();
        readInAllUsers();
        readInAllMessages();
        processCurrentChatroom();
    }
}

void ChatDaemon::changeChatroom(Chatroom* new_cur_chatroom) {
    m->lock();
    current_chatroom = new_cur_chatroom;
    m->unlock();
}

void ChatDaemon::sendMessage(string text) {
    Message* new_message = new Message(text, local_user->getUUID(), current_chatroom->getChatroomIndex());
    message_map[hash(text)] = new_message;
    //I think we should only need to create the message here
    //once it's created it's sent out via opensplice
    //from there it will be treated as any message
    //read in and processed in the right chatroom
}

int ChatDaemon::hash(string key_string) {
    int total = 0;
    
    for(int i = 0; i < key_string.length(); i++)
        total = total + (int) key_string[i];
    
    return total;
}

void ChatDaemon::readInPreviousUsers() {
    /*some reading in files from cout and stuff */
}

void ChatDaemon::postUsersToFile() {}

void ChatDaemon::exit() {}


