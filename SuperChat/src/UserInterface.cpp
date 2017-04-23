#include "../include/UserInterface.h"
#include "../include/ChatDaemon.h"
#include "../include/globals.h"


void UserInterface::create() {
    cout << "Inside UI\n";

    //local_user = daemon->addNewLocalUser("Tim");

    string nick; 
    //const char* nick; 
    //char curCR[] = "(Current Chatroom)\0"; //place holder for current user chatroom

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
/*<<<<<<< HEAD
    cbf[0] = new_field(30,(MAX_MESSAGE_LENGTH/3),8,2,0,0);    //creating chatbox field
    cbf[1] = NULL;               
=======*/
    cbf[0] = new_field(30,(MAX_MESSAGE_LENGTH/3),8,2,0,0);      //creating chatbox field
    cbf[1] = NULL;			
    crf[0] = new_field(10,(CHATROOMS_NAME_MAX+1),3,78,0,0);     //creating chatroom list field 
    crf[1] = NULL;    	     
    //ulf[0] = new_field(20,(8+1),15,53,0,0);                     //(megan) creating user list field (?)
    //ulf[1] = new_field(20,(8+1),15,88,0,0);
    //ulf[2] = NULL;
    on_ulf[0] = new_field(20,(8+1),15,53,0,0); 
    on_ulf[1] = NULL; 
    off_ulf[0] = new_field(20,(8+1),15,88,0,0);
    off_ulf[1] = NULL; 



//>>>>>>> b5e81247ebaf39065c513d367c2e9386dc7e71dd

    field_opts_off(mbf[0], O_AUTOSKIP);    //setting message box options
    field_opts_on(mbf[0], O_WRAP); 	      //"

/*<<<<<<< HEAD
    field_opts_off(cbf[0], O_AUTOSKIP);        //setting chatbox options
    field_opts_off(cbf[0], O_STATIC);            //"    
    field_opts_on(cbf[0], O_WRAP);         //"
=======*/
    field_opts_off(cbf[0], O_AUTOSKIP);    //setting chatbox options
    field_opts_off(cbf[0], O_STATIC);         //"    
    field_opts_on(cbf[0], O_WRAP);	      //"
//>>>>>>> b5e81247ebaf39065c513d367c2e9386dc7e71dd

    field_opts_off(crf[0], O_AUTOSKIP);	   //setting chatroom list options
    
    field_opts_off(ulf[0], O_AUTOSKIP);     
    field_opts_off(ulf[0], O_STATIC);

/*<<<<<<< HEAD
    msgbox = new_form(mbf);   //declaring forms
    post_form(msgbox);        //"
    chatbox = new_form(cbf);      //"
    post_form(chatbox);       //"
=======
    field_opts_off(ulf[1], O_AUTOSKIP);
    field_opts_off(ulf[1], O_STATIC);*/
    
    msgbox = new_form(mbf);		//declaring forms
    post_form(msgbox);			  //"
    chatbox = new_form(cbf);		  //"
    post_form(chatbox);			  //"
    chatroomList = new_form(crf);
    post_form(chatroomList);
   // usersList = new_form(ulf);
   // post_form(usersList);

    onlineUsersList = new_form(on_ulf); 
    post_form(onlineUsersList); 

    offlineUsersList = new_form(off_ulf); 
    post_form(offlineUsersList); 

//>>>>>>> b5e81247ebaf39065c513d367c2e9386dc7e71dd
    refresh();
    
    m->lock();
    daemon->setChatbox(chatbox);
    m->unlock();

//<<<<<<< HEAD

/////this section is for the local user to enter nick and log on
    attron(A_STANDOUT);                  
    mvprintw(1, 1, "Enter Nick: ");     

    while ((!daemon->LocalUserInitialized) && ((ch=getch()) != 8)){
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
                     daemon->addNewLocalUser(input); 
                     local_user = daemon->getLocalUser();    
                     
                     //mvprintw(0, 20,  "%s: ", local_user->getNick().c_str()); //print user nick 
                     //mvprintw(0, 10, "%s  ", local_user->getUUIDchar()); //print UUID

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
                     if(count < 8){                 //i think nick needs to be strictly < 8
                       form_driver(msgbox, ch);     //there's something up with opsl idl nick
                       input[count] = ch;           //where the 8th character must be null. 
                         if(count < 8)              //I'm not sure how to actually enforce this
                           count++;                 //cuz i don't know ncurses much at all. 
                         refresh();
                     }
                     break;
            }
         }
    }
      /*

      attron(A_BOLD);                      //drawing horizontal lines
      move(1,0); hline('_', C_MAX-1);       //"
      move(6,0); hline('_', 51);          //"
      move(38,0); hline('_', 51);         //"
      move(40,0); hline('_', C_MAX-1);

      
      move(2, 51); vline('|', 37);         //drawing vertical lines
      move(0,0); vline('|', 39);    //"
      move(0, C_MAX-1); vline('|', 39); //"

      attron(A_STANDOUT);                  //printing titles
      mvprintw(7, 1, "Chatbox:");      //"
      mvprintw(2, 1, "Enter Message: ");     //"
      mvprintw(0, 1, "SuperChat:");      //"
      mvprintw(2, 52, "Controls");       //"

      attroff(A_STANDOUT);

      mvprintw(3, 54, "'ESC' to exit client"); //"
      mvprintw(4, 54, "'F1' to ________");     //"
      mvprintw(5, 54, "'F2' to ________");     //"
      mvprintw(6, 54, "'F3' to ________");     //"
      mvprintw(7, 54, "'F4' to ________");     //"

      //attroff(A_BOLD);
      //mvprintw(0, 11, " %s: %s", nick, "02020\0"); //print user nick and UUID


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
                 mvprintw(0, 11, "%s: ", local_user->getNick().c_str()); //print user nick and UUID
                 mvprintw(0, 19, "%s  ", local_user->getUUIDchar()); //print user nick and UUID

               //printMessage(local_user, input, chatbox); //print function to print input[] into chatbox
                 daemon->setMessageLengthCounter(count);
                 daemon->sendMessage(input);
                 count=0;       
                 form_driver(msgbox, REQ_CLR_FIELD);
                 refresh(); 
                 break;

           case KEY_PPAGE:
               form_driver(chatbox, REQ_SCR_BLINE);
//=======*/
  attron(A_BOLD);                      //drawing horizontal lines
  move(1,0); hline('_', C_MAX-1);       //"
  move(6,0); hline('_', 51);	        //"	
  move(38,0); hline('_', C_MAX-1);      //"
  move(13,52); hline('_', 68);  	//"
  move(36,52); hline('_', 68);		//"

  move(2, 51); vline('|', 37);	       //drawing vertical lines
  move(0,0); vline('|', 39);		//"
  move(0, C_MAX-1); vline('|', 39);	//"
  move(2, 76); vline('|', 12);		//"
  move(14, 86); vline('|', 23);
  

  attron(A_STANDOUT);                  //printing titles
  mvprintw(7, 1, "Chatbox:");		    //"
  mvprintw(2, 1, "Enter Message: ");	    //"
  mvprintw(0, 1, "SuperChat:");		    //"
  mvprintw(2, 52, "Controls");		    //"
  mvprintw(2, 77, "Chatrooms");	            //"
  mvprintw(14, 52, "Online Users"); 	    //"
  mvprintw(14, 87, "Offline Users");	    //"

  attroff(A_STANDOUT);

  mvprintw(3, 52, "'ESC' to exit client");  //"
  mvprintw(4, 52, "'F1' Create chat room"); //"
  mvprintw(5, 52, "'F2' to ________");      //"
  mvprintw(6, 52, "'F3' to ________");	    //"
  mvprintw(7, 52, "'F4' to ________");      //"

 // attroff(A_BOLD);
  //  mvprintw(0, 11, " %s", nick); //print user nick and UUID
  

    while((ch=getch())!=27)
    {  switch(ch)
     {     case KEY_F(2):
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
              attron(A_STANDOUT);                  
              mvprintw(1, 1, "Enter Chatroom Name: "); 
              break; 

           case KEY_DOWN:
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
//>>>>>>> b5e81247ebaf39065c513d367c2e9386dc7e71dd
               break;

           case KEY_NPAGE:
               form_driver(chatbox, REQ_SCR_FLINE);
               break;
           case 10: //enter is pressed
              //  mvprintw(22, 11, "%s: ", local_user->getNick().c_str()); //print user nick and UUID
              // mvprintw(22, 19, "%s  ", local_user->getUUIDchar()); //print user nick and UUID

                //printMessage(local_user, input, chatbox); //print function to print input[] into chatbox
                daemon->setMessageLengthCounter(count);
                daemon->sendMessage(input);
                count=0;       
                form_driver(msgbox, REQ_CLR_FIELD);
                refresh(); 
           break;
             default:
               // mvprintw(22, 11, "%s: ", local_user->getNick().c_str()); //print user nick and UUID
               // mvprintw(22, 19, "%s  ", local_user->getUUIDchar()); //print user nick and UUID
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
/*<<<<<<< HEAD
      unpost_form(msgbox);
      unpost_form(chatbox);
      free_form(msgbox);
      free_form(chatbox);
      free_field(mbf[0]);
      free_field(cbf[0]); 
      //>>SEND LOGOUT SIGNAL TO CHATDAEMON HERE<<

      endwin();
      delete daemon;
      //This will totally cause errors once we have the file reading in place
=======*/
    //}
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
//>>>>>>> b5e81247ebaf39065c513d367c2e9386dc7e71dd
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

  form_driver(chatroomList, REQ_CLR_FIELD);
  for(i=0; i < chatrooms.size(); i++) {
    temp = chatrooms[i]->getName();
      for(j=0; j<temp.length(); j++){
        form_driver(chatroomList, temp[j]);
      }
    form_driver(chatroomList, REQ_NEXT_LINE);
  }
  refresh();
}

void UserInterface::printUsers(vector<User*> online, vector<User*> offline){
  //form_driver(usersList, REQ_PREV_FIELD);
  //form_driver(usersList, REQ_CLR_FIELD);
  form_driver(onlineUsersList, REQ_PREV_FIELD); 
  form_driver(onlineUsersList, REQ_CLR_FIELD); 
  string temp;
  int i, j;
  for(i=0; i<online.size(); i++){
    temp = online[i]->getNick();
      for(j=0; j<temp.length(); j++){
        form_driver(onlineUsersList, temp[j]);
  //    form_driver(usersList, temp[j]);
      }
    form_driver(onlineUsersList, REQ_NEXT_LINE);
  //form_driver(usersList, REQ_NEXT_LINE);
  }
  form_driver(offlineUsersList, REQ_NEXT_FIELD);
  form_driver(offlineUsersList, REQ_CLR_FIELD);
  //form_driver(usersList, REQ_NEXT_FIELD);
  //form_driver(usersList, REQ_CLR_FIELD);

  for(i=0; i<offline.size(); i++){
    temp = offline[i]->getNick();
      for(j=0; j<temp.length(); j++){
        form_driver(offlineUsersList, temp[j]);
     // form_driver(usersList, temp[j]);
      }
    form_driver(offlineUsersList, REQ_NEXT_LINE);
  //form_driver(usersList, REQ_NEXT_LINE);

  }
}



void UserInterface::setDaemon(ChatDaemon* new_daemon) {daemon = new_daemon;}

void UserInterface::setMutex(mutex* new_m) {m = new_m;}
