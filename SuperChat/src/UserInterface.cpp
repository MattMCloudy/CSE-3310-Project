/*Matt:
 * This is where all of the methods for the UserInterface will be implemented.
 * :)
 */

#include "../include/UserInterface.h"
#include "../include/ChatDaemon.h"
#include "../include/globals.h"


void UserInterface::create() {
    cout << "Inside UI\n";

    local_user = daemon->addNewLocalUser("Default");


    char nick[8]; 		//place holder for user nick string
    strncpy(nick, local_user->getNick().c_str(), sizeof(nick));

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
    cbf[0] = new_field(30,(MAX_MESSAGE_LENGTH/3),8,2,0,0);      //creating chatbox field
    cbf[1] = NULL;			
    crf[0] = new_field(10,(CHATROOMS_NAME_MAX+1),3,78,0,0);     //creating chatroom list field 
    crf[1] = NULL;    	     
    ulf[0] = new_field(20,(8+1),15,53,0,0);
    ulf[1] = new_field(20,(8+1),15,88,0,0);
    ulf[2] = NULL;
    inf[0] = new_field(1,(61),37,59,0,0);
    inf[1] = NULL;

    field_opts_off(mbf[0], O_AUTOSKIP);    //setting message box options
    field_opts_on(mbf[0], O_WRAP); 	      //"

    field_opts_off(cbf[0], O_AUTOSKIP);    //setting chatbox options
    field_opts_off(cbf[0], O_STATIC);         //"    
    field_opts_on(cbf[0], O_WRAP);	      //"

    field_opts_off(crf[0], O_AUTOSKIP);	   //setting chatroom list options
    field_opts_off(crf[0], O_STATIC);         //"    
    
    field_opts_off(ulf[0], O_AUTOSKIP);    
    field_opts_off(ulf[0], O_STATIC);

    field_opts_off(ulf[1], O_AUTOSKIP);
    field_opts_off(ulf[1], O_STATIC);

    field_opts_off(inf[0], O_AUTOSKIP);
    set_field_back(inf[0], A_UNDERLINE);
    
    msgbox = new_form(mbf);		//declaring forms
    post_form(msgbox);			  //"
    chatbox = new_form(cbf);		  //"
    post_form(chatbox);			  //"
    chatroomList = new_form(crf);
    post_form(chatroomList);
    usersList = new_form(ulf);
    post_form(usersList);
    userInput = new_form(inf);
    post_form(userInput);
    refresh();
    
    m->lock();
    daemon->setChatbox(chatbox);
    m->unlock();

  attron(A_BOLD);                      //drawing horizontal lines
  move(1,0); hline('_', C_MAX-1);       //"
  move(6,0); hline('_', 51);	        //"	
  move(38,0); hline('_', 52);           //"
  move(13,52); hline('_', 68);  	//"
  move(36,52); hline('_', 68);		//"

  move(2, 51); vline('|', 37);	       //drawing vertical lines
  move(0,0); vline('|', 39);		//"
  move(0, C_MAX-1); vline('|', 39);	//"
  move(2, 76); vline('|', 12);		//"
  move(14, 86); vline('|', 23);
  move(38,51); hline('/',70);

  attron(A_STANDOUT);                  //printing titles
  mvprintw(7, 1, "Chatroom:");		    //"
  mvprintw(2, 1, "Enter Message: ");	    //"
  mvprintw(0, 1, "SuperChat:");		    //"
  mvprintw(2, 52, "Controls");		    //"
  mvprintw(2, 77, "All Chatrooms");	    //"
  mvprintw(14, 52, "Online Users"); 	    //"
  mvprintw(14, 87, "Offline Users");	    //"

  attroff(A_STANDOUT);

  mvprintw(37, 52, "Input: "); 		    //"

						
  mvprintw(3, 52, "'ESC' to Exit Client");          //Printing Controls
  mvprintw(4, 52, "'Arw(Up/Dn' Scr Users");          //"
  mvprintw(5, 52, "'Arw(L/R)' Swp Usr List");        //"
  mvprintw(6, 52, "'Pg(Up/Dn)' to Scr CR");          //"
  mvprintw(8, 52, "*F2* to Create Chatroom");        //"
  mvprintw(9, 52, "*F3* to Change Nick");            //"
  mvprintw(10, 52, "*F4* to Change Chatroom");       //"
  mvprintw(11, 52, "*F5* to Rename Chatroom");       //"

  attroff(A_BOLD);
    mvprintw(0, 11, " %s", nick); //print user nick
  

    while((ch=getch())!=27)
    {  switch(ch)
     {     case KEY_DOWN:
             form_driver(usersList, REQ_SCR_FLINE);
             break;
 
           case KEY_UP:
             form_driver(usersList, REQ_SCR_BLINE);
             break;

	   case KEY_RIGHT:
	     form_driver(usersList, REQ_FIRST_FIELD);
	     form_driver(usersList, REQ_NEXT_FIELD);
	     break;

	   case KEY_LEFT:
	     form_driver(usersList, REQ_FIRST_FIELD);
	     break;

           case KEY_BACKSPACE:
             if(function==0){
	       if(count>=MAX_MESSAGE_LENGTH)
	         form_driver(msgbox, REQ_DEL_CHAR);
	       else {
                 form_driver(msgbox, REQ_DEL_PREV);
               }
	       if(count!=0)
	         count--;
	     }
	     if(function>0&&count!=0){
	       form_driver(userInput, REQ_DEL_PREV);
	       input.erase(input.length()-1,1);
	       count--;
             }
             break;

           case 10: //enter is pressed
	     if(function==0){
	       form_driver(msgbox, REQ_END_FIELD);
               daemon->setMessageLengthCounter(count);
               daemon->sendMessage(msg);
               count=0;       
               form_driver(msgbox, REQ_CLR_FIELD);
               refresh(); 
	     }
             if(function>0){
	       form_driver(chatroomList, REQ_NEXT_LINE);
	       form_driver(userInput, REQ_CLR_FIELD);
	       form_driver(msgbox, REQ_END_FIELD);
	       
 	       if(function==2)
		 daemon->createNewChatroom(input);    		   //**
	       if(function==3){
		 local_user = daemon->changeLocalUserNick(input);  //**
		 strncpy(nick, local_user->getNick().c_str(), sizeof(nick));
		
		 mvprintw(0, 11, "         ");
		 mvprintw(0, 11, " %s", nick);			   //print user nick
		 	
	       }
	       if(function==4){
		 daemon->changeChatroom(input);    		   //**
                 if(!(daemon->isError())){
		   form_driver(chatbox, REQ_NEXT_LINE); form_driver(chatbox, REQ_BEG_LINE);
		   mvprintw(7,11, input.c_str());
		   input = "       *Entered: " + input + "*";
		   for(i=0; i<input.length(); i++){
		     form_driver(chatbox, input[i]);
		   }
		   form_driver(chatbox, REQ_NEXT_LINE);
		 }
	       }
 	       form_driver(msgbox, REQ_END_FIELD);
	       function=0;
	       count=0;
	       input.clear();
	     }
	     form_driver(msgbox, REQ_END_FIELD);
             break;

	   case KEY_F(2):
	     function = 2;
	     form_driver(msgbox,REQ_CLR_FIELD);
	     form_driver(userInput, REQ_CLR_FIELD);
             count=0;
  	     break;

	   case KEY_F(3):
	     function = 3;
	     form_driver(msgbox,REQ_CLR_FIELD);
	     form_driver(userInput, REQ_CLR_FIELD);
	     count=0;
	     break;

	   case KEY_F(4):
	     function = 4;
	     form_driver(msgbox,REQ_CLR_FIELD);
	     form_driver(userInput, REQ_CLR_FIELD);
	     count=0;
	     break;

	   case KEY_PPAGE:
             form_driver(chatbox, REQ_SCR_BLINE);
	     break;

           case KEY_NPAGE:
	     form_driver(chatbox, REQ_SCR_FLINE);
	     break;

           default:
	     if(function==0){
               if(count<=MAX_MESSAGE_LENGTH){
	         form_driver(msgbox, ch);
	         msg[count] = ch;
               if(count<MAX_MESSAGE_LENGTH)
	         count++;
                 refresh();
               }
	     }
	     if(function==2||function==4){
               if(count<25){
                 form_driver(userInput,ch);
	         input.push_back(ch);
	         count++;
	         refresh();
	       }
	       form_driver(userInput, REQ_END_FIELD);
	     }	   
	     if(function==3){
               if(count<8){
                 form_driver(userInput,ch);
	         input.push_back(ch);
	         count++;
	         refresh();
	       }
	       form_driver(userInput, REQ_END_FIELD);
	     }	
             break;
      }
    }
    unpost_form(msgbox);
    unpost_form(chatbox);
    unpost_form(chatroomList);
    unpost_form(usersList);
    free_form(msgbox);
    free_form(chatbox);
    free_form(chatroomList);
    free_form(usersList);
    free_field(mbf[0]);
    free_field(cbf[0]); 
    free_field(crf[0]);
    free_field(ulf[0]);
    free_field(ulf[1]);
    endwin();
    delete daemon;
    //This will totally cause errors once we have the file reading in place
} 


void UserInterface::createChatroom(){
  string chatroom;
  char ch;
  int i=0;
  while((ch=getch())!=10){
    chatroom[i] = ch;
    
  }
}

void UserInterface::printMessage(User* origin_user, Message* new_message, int size)
{
    int i;
    int mssg_counter=0;
    static string lastUser = "";
    string prefix = "User: ";
    string nick = origin_user->getNick();
    string msg = new_message->getContent();
 
    form_driver(chatbox, REQ_END_FIELD);       //cursor to end of chatbox
    form_driver(chatbox, REQ_NEXT_LINE);       //cursor to next line 
    
    if(lastUser!=nick)
    {
      form_driver(chatbox, REQ_NEXT_LINE);
      form_driver(chatbox, REQ_BEG_LINE);

      for(i=0; i<prefix.length(); i++)
        form_driver(chatbox, prefix[i]);

      for(i=0; i<((MAX_MESSAGE_LENGTH/3)-1); i++, mssg_counter++){
        if(nick[i] == NULL)
          break;
        form_driver(chatbox, nick[i]);
      }

      form_driver(chatbox, REQ_NEXT_LINE);
    }

    form_driver(chatbox, REQ_BEG_LINE);
    form_driver(chatbox, '-'); 
    for(i=0; i < size; i++){                 //iterates through to input[count]
      form_driver(chatbox, msg[i]);          //sets prints input[i] to Chatbox
    }

    lastUser = nick;
    refresh();
}

void UserInterface::printChatrooms(vector<Chatroom*> chatrooms){
  form_driver(chatroomList, REQ_CLR_FIELD);
  string temp;
  int i, j;
  for(i=0; i<chatrooms.size(); i++) {
    temp = chatrooms[i]->getName();
      for(j=0; j<temp.length(); j++){
        form_driver(chatroomList, temp[j]);
      }
    form_driver(chatroomList, REQ_NEXT_LINE);
  }
  refresh();
  form_driver(chatroomList, REQ_BEG_FIELD);
}


void UserInterface::printUsers(vector<User*> online, vector<User*> offline){
  form_driver(usersList, REQ_PREV_FIELD); form_driver(usersList, REQ_PREV_FIELD);
  form_driver(usersList, REQ_CLR_FIELD);
  string temp;
  int i, j;
  for(i=0; i<online.size(); i++){
    temp = online[i]->getNick();
      for(j=0; j<temp.length(); j++){
        form_driver(usersList, temp[j]);
      }
    form_driver(usersList, REQ_NEXT_LINE); form_driver(usersList, REQ_BEG_LINE);
  }

  form_driver(usersList, REQ_NEXT_FIELD);
  form_driver(usersList, REQ_CLR_FIELD);

  for(i=0; i<offline.size(); i++){
    temp = offline[i]->getNick();
      for(j=0; j<temp.length(); j++){
        form_driver(usersList, temp[j]);
      }
    form_driver(usersList, REQ_NEXT_LINE); form_driver(usersList, REQ_BEG_LINE);
  }

}

void UserInterface::printError(string message){
  int i;
  form_driver(userInput, REQ_CLR_FIELD);
  message = "Error: " + message;
  for(i=0; i<message.length(); i++){
    form_driver(userInput, message[i]);
  }
}

void UserInterface::setDaemon(ChatDaemon* new_daemon) {daemon = new_daemon;}

void UserInterface::setMutex(mutex* new_m) {m = new_m;}
