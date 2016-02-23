#ifndef _MCUR
#define _MCUR

typedef enum 
	{WRITTING, READING, HALT}State;


void screen_init(void);
int fifo_init(FILE, FILE, char*); 
static void update_display(void);
void screen_end(void);
char* getCommand(char*);
int  sendCommand(const char*);
int getResult(char*);
void printResult(char*);

#endif
