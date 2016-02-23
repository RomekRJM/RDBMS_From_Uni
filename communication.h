#ifndef _COMMUN
#define _COMMUN

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define Q_SIZE	 4096
#define COMMAND -1
#define MOFF "bye"
#define FIFO_KS "/tmp/server_input.fifo"
 

typedef struct{
	int PID;
	char Msg[Q_SIZE];
	//const char *Msg;
	

}Query;

void printQuery( char* when, Query t );
char* getFifoName(int, char[]);
char* itoa(int, char*, int);
char* strrev(char*);
//void commun_close(FILE *,);

#endif
