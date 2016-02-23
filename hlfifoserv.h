#ifndef _MSERV
#define _MSERV

#include <semaphore.h>
#include <pthread.h>
#include "communication.h"
#include "interpreter.h"

#define MAXCLIENT 32
#define PRAWA 0660
#define SEM_N "semaforek"
#define LOG "server.logs"

// stany wątku: FREE - wątek wolny, nic nie robi i nie ma przydzielonego klienta
// AWAITING - wątek ma przydzielonego klienta, ale aktualnie czeka na polecenia od niego
// BUSY - wątek ma przydzielonego klienta, aktualnie realizuje jego zapytania
typedef enum{
	FREE, FINISHED, AWAITING, BUSY
}StanWatku;

typedef struct{
	pthread_t thread;
	StanWatku stan;

}Watki;

typedef struct{
	int pid;
	char fifo_name[20];
	char Msg[Q_SIZE];
	FILE *fp;
	polecenie p[MAXQSIZE];
	int cb;
	
}Klienci;


FILE* fifoKS_init();
int fifoSK_init(int);
int client_exist(int); // zwróci 32 - kolejka klienta już istnieje, 0-31 - nie istnieje ( nr jaki sie przydziela
																		//klientowi w tablicy klienci ), -1 - kolejka pełna
sem_t* KSFSem_init();
void sendResults(int, char*);
Query readFifoKS(FILE*);
void cleanUpPol(polecenie[MAXQSIZE]);
void remove_client( int nr );
void *servant(int);
void wtk_init(void);


#endif
