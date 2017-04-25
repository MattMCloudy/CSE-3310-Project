#include "../include/User.h"


User::User(string passed_nick, string passed_description, int passed_chatroom_idx) {
    nick = passed_nick;
    description = passed_description;
    chatroom_idx = passed_chatroom_idx;
    uuid = newBoostUUID();
    uuid_str = to_string(uuid); 
    uuid_char = uuid_str.c_str(); 
    isOnline = true;
    start = clock();
    setEntityManager();
    makeNewUser();
    sendUser();      
}

User::User(struct user* new_user) {
    string nick_temp(new_user->nick);
    nick = nick_temp;
    chatroom_idx = new_user->chatroom_idx;
    isOnline = true;
    //string desc_temp(new_user->description); the idl doesn't have this yet for some reason.
    //description = desc_temp;
    uuid = new_user->uuid;
    user_struct = new_user;
    start = clock();
}


User::~User() {
    em.deleteWriter();
    em.deletePublisher();
    em.deleteSubscriber();
    em.deleteTopic();
    em.deleteParticipant();
}

void User::setEntityManager() {
    string u_top_temp = "user";
    char* topic_name = strdup(u_top_temp.c_str());
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
    struct user* new_user = (struct user*) malloc(sizeof(struct user)); //This is the same as messageInstance
    strncpy(new_user->nick, nick.c_str(), sizeof(new_user->nick));
    //cout << "passing uuid: " << uuid << "\n";
    new_user->uuid = uuid;
    //cout << new_user->uuid << "\n";
    new_user->chatroom_idx = chatroom_idx;
    user_struct = new_user;
}

unsigned long long User::newBoostUUID() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    unsigned long long x;
    memcpy(&x, &uuid, sizeof(x));
    return x;
}

void User::sendUser() {
    /*
    cout << "Inside sender...\n";
    cout << user_struct->nick << "\n";
    cout << user_struct->uuid << "\n";
    */

    ReturnCode_t status = Writer->write(*user_struct, HANDLE_NIL);
    checkStatus(status, "Writer::write");
}

int User::getChatroomIndex() {return chatroom_idx;}

unsigned long long User::getUUID() {return uuid;}

string User::getNick() {return nick;}

void User::changeNick(string new_nick) {
    nick = new_nick;
    strncpy(user_struct->nick, new_nick.c_str(), sizeof(user_struct->nick));
}


/*
    (megan) (4/22)
    The elapsed_time check is just set to some arbitrary amout of time. 
    I messed with it forever trying to get it to be a determined amount 
    but it's all funky. We can either "eye-ball" it and set it to whatever 
    feels right. or someone can mess with it more if they want to make it check
    if like 5 min (or however much time we decided for someone to be inactive
    and declared offline) has passed. 
*/
void User::checkIfOnline() {
    //time_t now;
    //time(&now); 
    now = clock(); 
    long double elapsed_time; 
    elapsed_time = double(now - start);//CLOCKS_PER_SEC; //how many seconds have elapsed
   // cout << "ELAPSED TIME: " << elapsed_time << endl;
   // cout << "NOW: "          << now << endl; 
   // cout << "START: "        << start << endl; 
   // cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << endl; 
    if (elapsed_time > 100000)                    //600 seconds in 10 minutes
        isOnline = false;    
    else
        isOnline = true; 
}

void User::setIsOnline() {
    //time_t start; 
    //time(&start); 
    //clock_t start; 
    start = clock(); 
    //start = now;
    checkIfOnline();
}


bool User::getIsOnline() {return isOnline;}

const char* User::getUUIDchar() {
    return uuid_char; 
}
