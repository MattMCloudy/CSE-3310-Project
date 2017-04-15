#include <ncurses.h>

int main(){
  initscr();				//initialize screen
  start_color();
  init_pair(1, COLOR_RED, COLOR_WHITE);
  attron(COLOR_PAIR(1));
  noecho();
  int maxMessageSize = 144, maxX = 50, maxY = 12,
      inputX=0, inputY=1, maxInputR=3, maxInputC=48,  
      chatboxX=0, chatboxY=(inputY+(inputY+maxInputR)+1), maxChatboxR=6, maxChatboxC=48;

  int c, i, count=0, shift, i_row; int input[maxMessageSize], cbLines=0;   //initializing variables
  

  mvprintw(inputY-1, inputX, "Enter Message: "); 
  mvprintw(chatboxY-1, chatboxX, "Chatroom: ");
  move(inputY+maxInputR, 0); hline('-', maxInputC+1);
  move(inputY-1, inputX+maxInputC); vline('|', maxInputR+1);

  while((c!=27)){			//Loops until 'ESC' is pressed
     count=shift=0;				
     i_row=inputY;
     do {				//do-while to take and show user input
	if(inputX+shift>=maxInputC){
	  i_row++;
	  shift=0;
	}
	if(i_row-inputY<maxInputR){
	  move(i_row,(inputX+shift));	//moves cursor to end of string being input
	  echo();		
          c=getch();			 //takes input character
	  noecho();
	  if(c==127){
            mvprintw(i_row, inputX+shift-1, " ");
	    mvprintw(i_row, inputX+shift, " ");
	    mvprintw(i_row, inputX+shift+1, " ");
	    shift-=2; count--;
	    input[count]=' ';
	  }
     	  else{
	    input[count]=c;		 //saves input character to input string
	    count++;
	  }
          shift++;			 //increments counter variable

	}else{
	  c=getch();
	}
     }while((c)!=10&&(c)!=27); 		 //input character != 'Enter' or 'ESC' resp.
     
     
     i_row=inputY;		         
     shift=0;
     for(i=0; i<count; i++){		   //Loop to clear input line and print input string
        if(inputX+shift>=maxInputC){
	  i_row++;
	  shift=0;
	}
	mvprintw(i_row, inputX+shift, " ");			//'Clears' input msg box
	mvprintw(chatboxY+cbLines, chatboxX+i+1, "%c", input[i]); 	//prints input string to cb
	shift++;
     }cbLines++;
  }
  endwin();
  return 0;
}
