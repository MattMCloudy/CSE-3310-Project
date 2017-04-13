#include <ncurses.h>

int main() {
		initscr(); /* This starts curses */
		printw("Hello World!"); /* Just prints Hello World */
		refresh(); /* Print it on the screen */
		getch(); /* Wait for user input */
		endwin(); /* Closes the window */

		return 0;
}
