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
    cout << "ChatDaemon has started\n";
}

void ChatDaemon::setUI(UserInterface* new_ui) {
    ui = new_ui;
}

