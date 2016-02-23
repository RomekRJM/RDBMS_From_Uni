#ifndef _MINTERPRET
#define _MINTERPRET

#define MAXQSIZE 14


typedef enum { CREATE_DATABASE, USE, SHOW_DATABASE, DROP_DATABASE, CREATE_TABLE, 
					SHOW_TABLE, DROP_TABLE, DELETE, UPDATE, SELECT, INSERT, PNULL } typ; 

typedef struct { 
	char slowo[32];
	typ t;

}polecenie; 


void launch( polecenie[MAXQSIZE], int*, char*);
char *strtoupper(char *, const char *, size_t );
int rozwal( polecenie[MAXQSIZE], char*);

#endif