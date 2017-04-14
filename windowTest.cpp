#include <ncurses.h>
#include <form.h>





int main(){
  
  FIELD *field[3];                        //creating field array
  FORM *my_form;
  
  int ch;
  char input[144];  
  int i;
  int count=0;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  resizeterm(40, 75);
  //refresh();
  field[0] = new_field(3,48,3,1,0,0);       //creating message box field
  field[1] = new_field(30,48,7,1,0,144);    //creating chatbox field
  field[2] = NULL;

  set_field_back(field[0], A_UNDERLINE);    //setting message box options
  field_opts_off(field[0], O_AUTOSKIP);        //"

  set_field_back(field[1], A_UNDERLINE);    //setting chatbox options
  field_opts_off(field[1], O_AUTOSKIP);        //"

  my_form = new_form(field);
  post_form(my_form);
  refresh();

  mvprintw(1, 1, "_SuperChat_");
  mvprintw(2, 1, "Enter Message: ");
  mvprintw(6, 1, "Chatbox: ");
  
  refresh();

  while((ch=getch())!=27)
  {  switch(ch)
     {   case KEY_DOWN:
            form_driver(my_form, REQ_NEXT_LINE);
            break;
 
         case KEY_UP:
            form_driver(my_form, REQ_PREV_LINE);
            break;

         case KEY_LEFT:
            form_driver(my_form, REQ_LEFT_CHAR);
            break;
    
         case KEY_RIGHT:
            form_driver(my_form, REQ_RIGHT_CHAR);
            break;

         case KEY_BACKSPACE:
            form_driver(my_form, REQ_DEL_PREV);
            break;

         case 10:                                   //enter is pressed
            form_driver(my_form, REQ_NEXT_FIELD);   //moves to chat box
            
            
	    for(i=0; i<count; i++){                 //iterates through to input[count]
              form_driver(my_form, input[i]);       //sets prints input[i] to Chatbox
            }
            count=0;                                //resets the count to 0
            form_driver(my_form, REQ_PREV_FIELD);
            form_driver(my_form, REQ_CLR_FIELD);
            refresh(); 
            break;

         default:
            form_driver(my_form, ch);
	    input[count] = ch;
            count++;
            
            refresh();
            break;
     }//resizeterm(40, 75);refresh();
  }
  unpost_form(my_form);
  free_form(my_form);
  free_field(field[0]);
  free_field(field[1]);
      
  endwin();
  return 0;
}
