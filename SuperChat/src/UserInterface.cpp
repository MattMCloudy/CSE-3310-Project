/*Matt:
 * This is where all of the methods for the UserInterface will be implemented.
 * :)
 */

#include "../include/UserInterface.h"
#include "../include/ChatDaemon.h"

void UserInterface::create() {
    cout << "Inside UI\n";
}

void UserInterface::setDaemon(ChatDaemon* new_daemon) {daemon = new_daemon;}

void UserInterface::setMutex(mutex* new_m) {m = new_m;}
