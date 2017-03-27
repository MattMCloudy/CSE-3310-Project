#include "../include/ChatDaemon.h"
#include "../include/UserInterface.h"

void ChatDaemon::start() {
    cout << "ChatDaemon has started\n";
}

void ChatDaemon::setUI(UserInterface* new_ui) {
    ui = new_ui;
}

