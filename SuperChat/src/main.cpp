/* Matt:
 * Alright welcome to a short tour of the code I've written so far.
 * I hope this will make it a little more clear how this works and,
 * what might need to be changed in the future.
 *
 * So here we've just started in main. The first thing we'll do
 * is instance an object of Initializer, after this the constructor
 * for Initializer will begin, setting up our ChatDaemon
 * and User Interface (that occurs in Initializer.cpp).
 * After that, we have init.exe() which will start our threads,
 * and lead us into ChatDaemon and UserInterface.
 * GO TO Initializer.cpp
 */

#include "../include/Initializer.h"

int main() {
    Initializer init;
    init.exe();
}
