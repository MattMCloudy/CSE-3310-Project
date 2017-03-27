#ifndef __INITIALIZER_H
#define __INITIALIZER_H

#include "ViewController.h"
#include "UserInterface.h"
#include "ChatDaemon.h"

using namespace std;

class Initializer {
    private:
        ChatDaemon daemon;
        UserInterface ui;
    public:
        Initializer();
        void exe();
};

#endif
