#include <unistd.h>
#include <curses.h>
#include "klient.h"
#include "communication.h"


int PID;
WINDOW *scr_out, *scr_in, *box_out, *box_in;
char FIFO_SK[Q_SIZE];


int main(void){
	 
   char currCmd[Q_SIZE], currRes[Q_SIZE];	// obecnie przetwarzane: zapytanie, odpowiedź
   State state=WRITTING;
	int again=1;
	
   PID= getpid();	
   
	screen_init();
   update_display();
   
   while ( again ) {
   	
		if ( !strcmp(currCmd,MOFF) )
			state=HALT;	

		switch(state){
		case WRITTING:
			getCommand( currCmd );

			if( currCmd[ strlen(currCmd)-1 ]!='/' ){ // kasowanie ca�ego zapytania
				if( sendCommand(currCmd)!=0 ){ wprintw(scr_out,"\n%$*)(*&%^$*(");
					again=0;
					break;
				}
				state=READING;
			}
			else state=WRITTING;
			break;
		case READING:
			getResult(currRes);
			printResult(currRes); 
			state=WRITTING;
			break;
		case HALT:
			getResult(currRes);
			again=0;
			break;

		}   
   }
	
   screen_end();
   //printf("Zapytanie: %s\n", currCmd);
   //printf("Wiadomo??: \nPID=%d\nMsg=%s", test.PID, test.Msg );

   return 0;
}


void screen_init(void) {

	int HEIGHT, WIDTH, SCR_IN_H, SCR_IN_W;
	static WINDOW *ekran;

   ekran = initscr();
   noecho();
   nodelay(ekran, FALSE);
   refresh();
   wrefresh(ekran);

	getmaxyx(ekran, HEIGHT,WIDTH);

	SCR_IN_H= 28;
	SCR_IN_W= WIDTH - 1;

	box_out = newwin(SCR_IN_H, SCR_IN_W, 0, 0);
	box_in= newwin( HEIGHT-SCR_IN_H, SCR_IN_W, SCR_IN_H, 0 );
	box(box_in, ACS_VLINE, ACS_HLINE);
	box(box_out, ACS_VLINE, ACS_HLINE);
	wrefresh(box_out);
	wrefresh(box_in);

	scr_out = newwin( SCR_IN_H-2, SCR_IN_W-5, 1, 3 );
	scr_in= newwin( HEIGHT-SCR_IN_H-2, SCR_IN_W-5, SCR_IN_H + 1, 3 );

	cbreak();
	keypad(scr_in, TRUE);
	scrollok(scr_in, TRUE);
	scrollok(scr_out, TRUE);
	mvaddstr(0,20,"= Results obtained from data-base-serwer =");
	mvaddstr(SCR_IN_H, 20,"= Command Field =");

}

static void update_display(void) {
   wrefresh(scr_in);
	wrefresh(scr_out);
   refresh();
}

void screen_end(void) {
   endwin();
}

char* getCommand(char *cmd){
	int MAX_L,MAX_L_LEN,cY=0, cX=0, i=0, end=1;

	getmaxyx(box_in, MAX_L, MAX_L_LEN);
	wmove(scr_in, cY, cX);

	wprintw(scr_out,"\n");

	while( end && i<Q_SIZE-1 ) {
		cmd[i] = wgetch(scr_in);
		end= ( ( ( cmd[i]==';' ) || ( cmd[i]=='/' ) )? 0:1 );
 		mvwaddch(scr_in,cY,cX,cmd[i]);
		refresh();

		if( cX < MAX_L_LEN && cmd[i]!=10 && end)

			++cX;	

		else{

			if( cY < MAX_L ){
				++cY;	
			}
			else{
				wscrl(scr_in,0);
			} 
			cX=0;

		}
		++i;
	}
   
	if( cmd[i-1] != '/'){

		cmd[i-1]='\0'; // null na ko�cu, zamiast ';' 
   	wprintw(scr_out,"Wysylam: %s", cmd);
	}
	else	wprintw(scr_out,"Usuwam zapytanie.");

	wclear(scr_in);
	update_display();

   return cmd;	
}


int sendCommand(const char cmd[]){

	Query command; 
	FILE *wfp;
	struct stat st;

	command.PID=PID;	sprintf(command.Msg, cmd);

	if(stat(FIFO_KS,&st) != 0){
		screen_end();
      printf("\nSerwer nie działa\n");
		return 1;	
	}

	if ( ( wfp= fopen(FIFO_KS, "wb") ) == NULL ){
		printf("\nBlad otwarcia kolejki fifo do zapisu");
		return 1;
	}

	//printf("\nZapis komunikatu do kolejki");
	
	if ( fwrite( &command, sizeof(command), 1, wfp ) < 0 ){
		printf("\nBlad zapisu");
		return 1;
	}
	
	fclose(wfp);
	if( !strcmp(MOFF, cmd) )	return 2;

	return 0;
	
}


int getResult(char* res){

	char name[Q_SIZE];
	FILE *rfp;
	Query result;

	sleep(1); 
	getFifoName(PID, name);	
	
	if ( ( rfp= fopen( name , "rb") ) == NULL ){
		printf("\nBlad otwarcia kolejki fifo do odczytu\n");
		return 1;
	}
	
	if ( fread( &result, sizeof(result), 1, rfp ) < 0 ){
		printf("\nBlad odczytu\n");
		return 1;
	}
	
	sprintf(res, result.Msg);
	//printQuery("\nOdebrano z servera :\n",result);
	fclose(rfp);

	return 0;

}

void printResult(char *res){

	wprintw(scr_out,"\nServer odpowiada:\n%s", res);
	update_display();

}


