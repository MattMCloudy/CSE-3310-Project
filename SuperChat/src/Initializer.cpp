#include "../include/Initializer.h"

Initializer::Initializer() {
    UserInterface new_ui;
    ChatDaemon new_daemon;
    
    daemon = new_daemon;
    ui     = new_ui;
}


void Initializer::exe() {
    cout << "Begin Execution...\n";


    thread daemon_thread(&ChatDaemon::start, ref(daemon));
    thread ui_thread(&UserInterface::create, ref(ui));

    daemon_thread.join();
    ui_thread.join();
    
}
