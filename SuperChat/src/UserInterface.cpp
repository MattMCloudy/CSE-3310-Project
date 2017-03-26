#include "../include/UserInterface.h"


void UserInterface::test() {
    initscr();
    printw("Hello, World");
    refresh();
    getch();
    endwin();
}

