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

void ChatDaemon::start() {
    m->lock();
    cout << "ChatDaemon has started\n";
    m->unlock();
}

void ChatDaemon::setUI(UserInterface* new_ui) {
    ui = new_ui;
}

void ChatDaemon::setMutex(mutex* new_m) {
    m = new_m;
}



Chatroom* createNewChatroom(string name) {
    //Trap from UI to here to create a new Chatroom 
    //possibly need more than name for params
    
    //Something like this should work for making chatrooms

    /*if (chatrooms.size() > 10) {
        cerr << "ERROR: Already 10 chatrooms initialized";
        return NULL;
      } else {
        Chatroom new_chatroom(name);
        chatrooms.append(&new_chatroom);
        changeChatroom(&new_chatroom);
        postChatroomToUI(&new_chatroom);
        waitForNewMessageReceived(&new_chatroom);
      }
    */
}

void postChatroomToUI(Chatroom* chatroom) {
    //Something like this should work for this method
    
    /*
    m->lock();
    ui->postNewChatroom(chatroom);
    m->unlock();
    */
    
    //Possibly in Chatroom instead?
}

void postNewMessageToUI(Message* new_message) {
   
    /*
    m->lock();
    ui->postNewMessage(message);
    m->unlock();
    */

    //This could also be inside of Chatroom theoretically.
}

LocalUser* addNewLocalUser(string nick) {
    
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

RemoteUser* addNewRemoteUser(RemoteUser* new_user) {
    
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

Message* sendNewMessage(string message_text) {

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

void waitForNewMessageReceived() {
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

void changeChatroom(Chatroom* new_cur_chatroom) {
  
    /*
    if (cur_chatroom != NULL) {
        m->lock();
        kill_current_chatroom = true;
        m->unlock();
    }
    
    cur_chatroom = new_cur_chatroom;
    */
}

void readInPreviousUsers() {
    /*some reading in files from cout and stuff */
}

void postUsersToFile() {}




