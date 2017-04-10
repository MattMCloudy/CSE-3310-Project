/*Matt:
 * OK, this is where all of the methods in ChatDaemon will be implemented.
 * I bet we'll have to get pretty cozy with this file.
 * Thanks for following along on this little tour, lmk any questions/concerns you
 * may have.
 * <3 you groupies.
 * Matt out.
 */


#include "../include/ChatDaemon.h"
#include "../include/UserInterface.h"

ChatDaemon::ChatDaemon() {}

void ChatDaemon::start() {
    m->lock();
    LocalUserInitialized = false;
    //build user list from file ** needs to be written
    readInPreviousUsers();
    m->unlock();
    
    //Update user list with DDS
    //Update message list with DDS
    //Update chatroom list with DDS
    
    readInAllUsers();
    readInAllMessages();
    readInAllChatrooms();
    
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

    if (chatrooms.size() == 0)
        createNewChatroom("public");
    else {
        changeChatroom(chatrooms[0]);
    }


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

    em.createParticipant("");

    userTypeSupport_var T_user = new userTypeSupport();
    em.registerType(T_user.in());
    em.createTopic(user_topic_name);

    messageTypeSupport_var T_message = new messageTypeSupport();
    em.registerType(T_message.in());
    em.createTopic(message_topic_name);

    chatroomTypeSupport_var T_chatroom = new chatroomTypeSupport();
    em.registerType(T_chatroom.in());
    em.createTopic(chatroom_topic_name);
    
    em.createSubscriber();

    em.createReader();
    DataReader_var dreader = em.getReader();
    user_reader = userDataReader::_narrow(dreader.in());
    message_reader = messageDataReader::_narrow(dreader.in());
    chatroom_reader = chatroomDataReader::_narrow(dreader.in());
    checkHandle(user_reader.in(), "MsgDataReader::_narrow");
    checkHandle(message_reader.in(), "MsgDataReader::_narrow");
    checkHandle(chatroom_reader.in(), "MsgDataReader::_narrow");
}

ChatDaemon::~ChatDaemon() {
    

}

void ChatDaemon::readInAllUsers() {
    userSeq userList;
    SampleInfoSeq infoSeq;
    ReturnCode_t status = -1;
    status = user_reader->take(userList, infoSeq, LENGTH_UNLIMITED, ANY_SAMPLE_STATE,
                            ANY_VIEW_STATE, ANY_INSTANCE_STATE);
    checkStatus(status, "MsgDataReader::take");
    
    for(ULong j = 0; j < userList.length(); j++) {
        User* new_user = new User(&userList[j]);
        users.push_back(new_user);
    }

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
        Chatroom* new_chatroom = new Chatroom(&chatList[j], this);
        chatrooms.push_back(new_chatroom);
    }

    status = chatroom_reader->return_loan(chatList, infoSeq);
    checkStatus(status, "MsgDataReader::return_loan");
}

Chatroom* ChatDaemon::createNewChatroom(string name) {
    //Trap from UI to here to create a new Chatroom 
    //possibly need more than name for params
    
    //Something like this should work for making chatrooms

    if (chatrooms.size() > 10) {
        cerr << "ERROR: Already 10 chatrooms initialized";
      } else {
        string name = "random";
        Chatroom new_chatroom(name, chatrooms.size(), this);
        chatrooms.push_back(&new_chatroom);
        //changeChatroom(&new_chatroom);
        //postChatroomToUI(&new_chatroom);
        return &new_chatroom;
      }
    return NULL;
}

void ChatDaemon::postChatroomToUI(Chatroom* chatroom) {
    //Something like this should work for this method
    
    /*
    m->lock();
    ui->postNewChatroom(chatroom);
    m->unlock();
    */
    
    //Possibly in Chatroom instead?
}

void ChatDaemon::postNewMessageToUI(Message* new_message) {
   
    /*
    m->lock();
    ui->postNewMessage(message);
    m->unlock();
    */

    //This could also be inside of Chatroom theoretically.
}

LocalUser* ChatDaemon::addNewLocalUser(string nick) {
    
    /*
    if (LocalUserInitialized) {
        cerr << "A local user has already been initialized: " << local_user->getNick();
        return local_user;
    }

    LocalUser new_local_user(nick);
    users.append(&new_local_user);
    local_user = &new_local_user;
    
    >>Only do this if the public chatroom has been initialized
    cur_chatroom->addNewUser(local_user);
    
    LocalUserInitialized = true;
    return local_user;
    */
}

RemoteUser* ChatDaemon::addNewRemoteUser(RemoteUser* new_user) {
    
    /*
    So we should be receiving the remote users from opensplice,
    I assume they will be initialized elsewhere and here we'll
    just take care of adding it to the list of users and posting
    it to UI
    */
    
    /*
    users.append(new_user);
    cur_chatroom->addNewUser(new_user); >>We may very well be getting this
                                        >>new user from chatroom, so this
                                        >>could be unnecessary

    >>we'll see how this works but we may need a mutex lock here
    m->lock();
    ui->postNewUser(new_user);
    m->unlock();

    return new_user;                    >>It could also be unnecessary to return anything here
    */
}

Message* ChatDaemon::sendNewMessage(string message_text) {

    /*
    Message new_message(message_text, local_user); >>I assume we'll need to give the
                                                   >>sender with the message
    cur_chatroom->sendMessage(&new_message);

    >>Once we return the Message, ui will post it immediately
    return &new_message;
    */
}

/* So I had a bit of a messy situation in the next
 * couple of methods.
 *
 * Essentially I needed way for a thread stuck in the
 * following while(true) to break out when necessary.
 *
 * The way I see it, I can kill the current chatrooms
 * connection to opensplice when we change chatrooms.
 * That way, the threads will change responsibilities.
 * The one stuck in this while(true) will return back
 * to UI and become the one listening to user input.
 * Then, the next time someone makes a new chatroom,
 * or changes chatrooms the one listening into user
 * input will shift to receving messages.
 */

void ChatDaemon::waitForNewMessageReceived() {
    /*
    Here we have some means of going into Chatroom
    and waiting for new messages to pop up.
    
    while(true) {
        if (kill_current_chatroom)
            break;
        
        Message* new_message = cur_chatroom->receiveMessages();
        postNewMessageToUI(new_message);
    }
    */
}

void ChatDaemon::changeChatroom(Chatroom* new_cur_chatroom) {
  
    /*
    if (cur_chatroom != NULL) {
        m->lock();
        kill_current_chatroom = true;
        m->unlock();
    }
    
    cur_chatroom = new_cur_chatroom;
    */
}

void ChatDaemon::readInPreviousUsers() {
    /*some reading in files from cout and stuff */
}

void ChatDaemon::postUsersToFile() {}




