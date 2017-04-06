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
    //
    //if (chatrooms.size() > 10) {
    //    cerr << "ERROR: Already 10 chatrooms initialized";
    //    return NULL;
    //} else {
    //    Chatroom new_chatroom(name);
    //}
    //
}

void postChatroomToUI(Chatroom* chatroom); //Possibly in Chatroom instead?
void postNewMessageToUI(Message*); //This could also be inside of Chatroom theoretically.
LocalUser* addNewLocalUser(string nick);
RemoteUser* addNewRemoteUser(RemoteUser* new_user);
void sendNewMessage(Message* new_message);
void waitForNewMessageReceived();
void changeChatroom(Chatroom* new_cur_chatroom);
void readInPreviousUsers();
void postUsersToFile();


