/*Matt:
 * This is where all of the methods for the UserInterface will be implemented.
 * :)
 */

#include "../include/UserInterface.h"
#include "../include/ChatDaemon.h"
#include "../include/globals.h"


void UserInterface::create() {
    cout << "Inside UI\n";

    local_user = daemon->addNewLocalUser("Tim");


    const char* nick = local_user->getNick().c_str(); 		//place holder for user nick string
    char curCR[] = "(Current Chatroom)\0"; //place holder for current user chatroom

    //SEND LOCAL USER TO CHAT DAEMON

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    resizeterm(R_MAX, C_MAX);
    refresh();

    mbf[0] = new_field(3,(MAX_MESSAGE_LENGTH/3),3,2,0,0);       //creating message box field
    mbf[1] = NULL;
    cbf[0] = new_field(30,(MAX_MESSAGE_LENGTH/3),8,2,0,0);    //creating chatbox field
    cbf[1] = NULL;			
    crf[0] = new_field(10,(CHATROOMS_NAME_MAX+1),3,78,0,0);     //creating chatroom list field 
    crf[1] = NULL;    	     

    field_opts_off(mbf[0], O_AUTOSKIP);    //setting message box options
    field_opts_on(mbf[0], O_WRAP); 	      //"

    field_opts_off(cbf[0], O_AUTOSKIP);    //setting chatbox options
    field_opts_off(cbf[0], O_STATIC);         //"    
    field_opts_on(cbf[0], O_WRAP);	      //"

    field_opts_off(crf[0], O_AUTOSKIP);	   //setting chatroom list options
    //field_opts_off(crf[0], O_STATIC); 
    

    msgbox = new_form(mbf);		//declaring forms
    post_form(msgbox);			  //"
    chatbox = new_form(cbf);		  //"
    post_form(chatbox);			  //"
    chatroomList = new_form(crf);
    post_form(chatroomList);
    refresh();
    
    m->lock();
    daemon->setChatbox(chatbox);
    m->unlock();

  attron(A_BOLD);                      //drawing horizontal lines
  move(1,0); hline('_', C_MAX-1);       //"
  move(6,0); hline('_', 51);	        //"	
  move(38,0); hline('_', 51);	        //"
  move(40,0); hline('_', C_MAX-1);      //"
  move(13,52); hline('_', 68);  

  move(2, 51); vline('|', 37);	       //drawing vertical lines
  move(0,0); vline('|', 39);		//"
  move(0, C_MAX-1); vline('|', 39);	//"
  move(2, 76); vline('|', 12);		//"

  attron(A_STANDOUT);                  //printing titles
  mvprintw(7, 1, "Chatbox:");		    //"
  mvprintw(2, 1, "Enter Message: ");	    //"
  mvprintw(0, 1, "SuperChat:");		    //"
  mvprintw(2, 52, "Controls");		    //"
  mvprintw(2, 77, "Chatrooms");	            //"

  attroff(A_STANDOUT);

  mvprintw(3, 52, "'ESC' to exit client");  //"
  mvprintw(4, 52, "'F1' Create chat room"); //"
  mvprintw(5, 52, "'F2' to ________");      //"
  mvprintw(6, 52, "'F3' to ________");	    //"
  mvprintw(7, 52, "'F4' to ________");      //"

  attroff(A_BOLD);
    mvprintw(0, 11, " %s", nick); //print user nick and UUID
  

    while((ch=getch())!=27)
    {  switch(ch)
     {     case KEY_DOWN:
               form_driver(msgbox, REQ_NEXT_LINE);
               break;
 
           case KEY_UP:
               form_driver(msgbox, REQ_PREV_LINE);
               break;

           case KEY_LEFT:
               form_driver(msgbox, REQ_LEFT_CHAR);
               break;
    
           case KEY_RIGHT:
               form_driver(msgbox, REQ_RIGHT_CHAR);
               break;

           case KEY_BACKSPACE:
            
	           if(count>=MAX_MESSAGE_LENGTH)
	               form_driver(msgbox, REQ_DEL_CHAR);
	           else {
	               form_driver(msgbox, REQ_DEL_PREV);
	           }
	           if(count!=0)
	               count--;
               break;

           case 10: //enter is pressed

	           //printMessage(local_user, input, chatbox); //print function to print input[] into chatbox
               daemon->setMessageLengthCounter(count);
               daemon->sendMessage(input);
               count=0;       
               form_driver(msgbox, REQ_CLR_FIELD);
               refresh(); 
               break;

	       case KEY_PPAGE:
	           form_driver(chatbox, REQ_SCR_BLINE);
	           break;

	       case KEY_NPAGE:
	           form_driver(chatbox, REQ_SCR_FLINE);
	           break;

           default:
               if(count<=MAX_MESSAGE_LENGTH){
	               form_driver(msgbox, ch);
	               input[count] = ch;
                   if(count<MAX_MESSAGE_LENGTH)
	                   count++;
                   refresh();
               }
               break;
      }
    }
    unpost_form(msgbox);
    unpost_form(chatbox);
    free_form(msgbox);
    free_form(chatbox);
    free_field(mbf[0]);
    free_field(cbf[0]); 
    free_field(crf[0]);

    endwin();
    delete daemon;
    //This will totally cause errors once we have the file reading in place
} 



void UserInterface::printMessage(User* origin_user, Message* new_message, int size)
{
    int i;
    int mssg_counter=0;
    static long long int lastUser = 0;
    string nick = origin_user->getNick();
    long long int ID = origin_user->getUUID();
    string input = new_message->getContent();
 
    form_driver(chatbox, REQ_END_FIELD);       //cursor to end of chatbox
    form_driver(chatbox, REQ_NEXT_LINE);       //cursor to next line 
    if(lastUser!=ID){
        for(i=0; i<((MAX_MESSAGE_LENGTH/3)-1); i++, mssg_counter++){
            if(nick[i] == NULL)
                break;
            form_driver(chatbox, nick[i]);
        }
        
        /*
        for(i=0; mssg_counter<((MAX_MESSAGE_LENGTH/3)-4); i++, mssg_counter++){
            if(nick[i] == NULL)
            break;
            form_driver(chatbox, ID[i]);
        }
        form_driver(chatbox, ']');
        */

        form_driver(chatbox, REQ_NEXT_LINE);form_driver(chatbox, REQ_NEXT_LINE); 
    }
    form_driver(chatbox, ':'); 
    for(i=0; i < size; i++){                 //iterates through to input[count]
        form_driver(chatbox, input[i]);       //sets prints input[i] to Chatbox
    }

    lastUser = ID;
  refresh();
}

void UserInterface::printChatrooms(vector<Chatroom*> chatrooms){
  string temp;
  int i, j;
  for(i=0; i < chatrooms.size(); i++) {
    temp = chatrooms[i]->getName();
      for(j=0; j<temp.length(); j++){
        form_driver(chatroomList, temp[i]);
      }
    form_driver(chatroomList, REQ_NEXT_LINE);
  }
  refresh();
}

void UserInterface::setDaemon(ChatDaemon* new_daemon) {daemon = new_daemon;}

void UserInterface::setMutex(mutex* new_m) {m = new_m;}
