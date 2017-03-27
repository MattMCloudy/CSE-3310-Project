#include "../include/UserInterface.h"
#include "../include/ChatDaemon.h"

void UserInterface::create() {
    initscr();
    printw("Hello, World");
    refresh();
    getch();
    endwin();
}

void UserInterface::setDaemon(ChatDaemon* new_daemon) {daemon = new_daemon;}
