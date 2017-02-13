#include <form.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>


#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char* choices[] = {"Continue App","Exit"};

int main()
{	FIELD *field[5];
	FORM  *my_form;
	ITEM** my_items;
	MENU* my_menu;
	ITEM* cur_item;
	int cur_field = 0;
	int ch, n_choices, i;
	bool onExit = false;
	
	/* Initialize curses */
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	start_color();

	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

	for(i = 0; i < n_choices; ++i)
	        my_items[i] = new_item(choices[i], choices[i]);
	my_items[n_choices] = (ITEM *)NULL;

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);

	/* Initialize the fields */
	field[0] = new_field(1, 10, 4, 18, 0, 0); /* y = 4, color = red  */
	field[1] = new_field(1, 10, 16, 18, 0, 0);/* y = 16, color = red */
	field[2] = new_field(1, 10, 4, 18, 0, 0); /* y = 4, color = blue */
	field[3] = new_field(1, 10, 16, 18, 0, 0);/* y = 16, color = blue */
	field[4] = NULL;

	/* Set field options */
	set_field_back(field[0], A_UNDERLINE); 	/* Print a line for the option 	*/
	field_opts_off(field[0], O_AUTOSKIP);  	/* Don't go to next field when this */
						/* Field is filled up 		*/
	set_field_back(field[1], A_UNDERLINE); 
	field_opts_off(field[1], O_AUTOSKIP);
	

	set_field_back(field[2], A_UNDERLINE); 
	field_opts_off(field[2], O_AUTOSKIP);
	
	set_field_back(field[3], A_UNDERLINE); 
	field_opts_off(field[3], O_AUTOSKIP);

	set_field_fore(field[0], COLOR_PAIR(1));
	set_field_fore(field[1], COLOR_PAIR(1));
	set_field_fore(field[2], COLOR_PAIR(2));
	set_field_fore(field[3], COLOR_PAIR(2));
	set_field_back(field[0], COLOR_PAIR(1));
	set_field_back(field[1], COLOR_PAIR(1));
	set_field_back(field[2], COLOR_PAIR(2));
	set_field_back(field[3], COLOR_PAIR(2));
	/* Create the form and post it */
	my_form = new_form(field);
	post_form(my_form);
	refresh();

	my_menu = new_menu((ITEM **)my_items);
	post_menu(my_menu);
	refresh();

	set_current_field(my_form, field[0]);
	set_current_item(my_menu, my_items[0]);
	mvprintw(4, 10, "Value 1:");
	mvhline(10, 0, '-', 200);
	mvprintw(16, 10, "Value 2:");
	refresh();

	/* Loop through to get user requests */
	while((ch = getch()) != KEY_F(3))
	{	
		switch(ch)
		{	case KEY_F(2):
				/* Change Placement:
				 * 0->1
				 * 1->0
				 * 2->3
				 * 3->2 */
				
				if (cur_field == 0) {
					set_current_field(my_form, field[1]);
					cur_field = 1;
				} else if (cur_field == 1) {
					set_current_field(my_form, field[0]);
					cur_field = 0;
				} else if (cur_field == 2) {
					set_current_field(my_form, field[3]);
					cur_field = 3;
				} else if (cur_field == 3) {
					set_current_field(my_form, field[2]);
					cur_field = 2;
				}

				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_F(1):
				/* Change Color:
				 * 0->2
				 * 1->3
				 * 2->0
				 * 3->1 */
				
				if (cur_field == 0) {
					set_current_field(my_form, field[2]);
					cur_field = 2; 
				} else if (cur_field == 1) {
					set_current_field(my_form, field[3]);
					cur_field = 3;
				} else if (cur_field == 2) {
					set_current_field(my_form, field[0]);
					cur_field = 0;
				} else if (cur_field == 3) {
					set_current_field(my_form, field[1]);
					cur_field = 1;
				}

				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				onExit = true;
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				onExit = false;
				break;
			case 10:
				if (onExit) {
					unpost_form(my_form);
					unpost_menu(my_menu);
					free_form(my_form);
					free_menu(my_menu);				
					free_field(field[0]);
					free_field(field[1]); 
					free_field(field[2]);
					free_field(field[3]);
					free_item(my_items[0]);
					free_item(my_items[1]);
					endwin();
					exit(0);
				}
				break;
			default:
				/* If this is a normal character, it gets */
				/* Printed				  */	
				form_driver(my_form, ch);
				break;
		}
	}

	/* Un post form and free the memory */
	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
	free_field(field[1]); 
	free_field(field[2]);
	free_field(field[3]);
	endwin();
	return 0;
}
