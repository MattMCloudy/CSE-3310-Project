/*Matt:
 *Alright, now we're in Initializer.cpp, here you can see the constructor
 *for Initializer which for right now, just instances two objects of ChatDaemon
 *and UserInterface. Then, it passes those two objects into the private variables
 *daemon and ui (which can be found in Initialzer.h).
 *After that we have our threading, mind you right now I haven't implemented
 *any kind of mutex so it will hit race conditions when they try
 *to enter the same critical region (printing, calls to the other class
 *will cause this). However, the mutex library is included already
 *so feel free to start including them. They are different however,
 *from the pthread mutexes (way easier honestly) so hit up tutorials point
 *or whatever before you go crazy. Another note, threading and mutexing in
 *c++ were implemented this way in c++11 the Makefile I have in the root
 *directory already includes this and the flags for the threading, just
 *be aware that it may get pissed off at you if you try to use some old compiler
 *in an IDE.
 *
 *Megan GO TO ../include/ChatDaemon.h
 *Nima  GO TO ../include/UserInterface.h
 */

#include "../include/Initializer.h"

Initializer::Initializer() {
   // UserInterface new_ui;
    ChatDaemon new_daemon;
    
    daemon = new_daemon;
 //   ui     = new_ui;
}


void Initializer::exe() {
    cout << "Begin Execution...\n";
    
    mutex m;
    
    daemon.setMutex(&m);
   // ui.setMutex(&m);
    
   // daemon.setUI(&ui);
  //  ui.setDaemon(&daemon);

    cout << "Entering daemon thread...\n";
    thread daemon_thread(&ChatDaemon::start, ref(daemon));
 //   cout << "Entering ui thread...\n";
  //  thread ui_thread(&UserInterface::create, ref(ui));
    

  //  ui_thread.join();
    daemon_thread.join();
}
