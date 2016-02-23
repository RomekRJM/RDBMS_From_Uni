#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "hlfifoserv.h"


Klienci klienci[MAXCLIENT];
Watki watki[MAXCLIENT];
int STOP=0;

int main(){

	Query a;
	int isClient,i,create;
	FILE *rfp;

	if( read_base()<0 ){
		init_db();
		write_base();
	}

	if( ( rfp=fifoKS_init() ) == NULL){
		printf("Blad otwarcia kolejki FIFO do odczytu");
		return 1;
	}

	wtk_init();

 while(!STOP){
	printf("\n****************************************");
	a=readFifoKS(rfp);
	isClient=client_exist(a.PID);

	if( !strcmp(a.Msg, MOFF) ){
		STOP=1;
		printf("\n******** SERWER  ZOSTANIE  ZAMKNIĘTY *******\n");
	}

	if(!STOP){
		if( isClient>=0 ){
	
			if( isClient>=MAXCLIENT){ 
				isClient-=32;
				create=0;
			}
			else create=1;
	
			sprintf(klienci[isClient].Msg, a.Msg);
			watki[isClient].stan=BUSY;
	
			if( create ){
			
				printf("\n++++++++++++++++++++++++++++++++++++++%d", isClient);
				fifoSK_init(isClient);
				//daj mu wątek
				pthread_create(&watki[isClient].thread, NULL, &servant, isClient);
			}
		}			
	}
	else{
		for(i=0; i<MAXCLIENT; ++i){
			if(watki[i].stan > FINISHED){
				sprintf(klienci[i].Msg, MOFF); 
				watki[i].stan= BUSY;
				printf("\n%d stop >> %s", i, a.Msg); 
				printf("\n%d stan= %d", i,  watki[i].stan); 
			} 		
		} 

	}

	for(i=0; i<MAXCLIENT; ++i){
		if(STOP)printf("\nStan watku %d przy konczeniu = %d", i, watki[i].stan);

		if(watki[i].stan==FINISHED){
			pthread_join(watki[i].thread, NULL);
			watki[i].stan= FREE; printf("\nZakonczono watek %d, zwolniono jego zasoby.", i);
		}
		// kolejka pełna - olej klienta
	}

	}

	write_base();
	fclose(rfp); 
	unlink(FIFO_KS);

	return 0;
}

FILE* fifoKS_init(){

	unlink(FIFO_KS); 

	if ( mknod(FIFO_KS, S_IFIFO | PRAWA, 0) < 0 ){
		printf("\nBlad tworzenia kolejki fifo");
		return NULL;
	}

	return fopen(FIFO_KS, "rb");

}

void wtk_init(void){

	int i;

	for(i=0; i<MAXCLIENT; ++i)
		watki[i].stan=FREE;

}


Query readFifoKS(FILE* fp){
	
	int k=0;
	Query a;

	while( k==0 )	// dopuki nic nie odczytałeś blokuj i próbuj dalej
		k=fread(&a, sizeof(Query), 1, fp);
	
	printQuery("\nOdczytano z kolejki:\t",a);

	return a;
}

int client_exist( int pid ){

	int i, index_pustego=-1, puste_miejsca=MAXCLIENT;
	char name[Q_SIZE];

	for(i=0; i<MAXCLIENT; ++i){
		
		if( klienci[i].pid==0 ){

			if( index_pustego==-1 ){
				index_pustego=i;
			}
		
		}
		else if ( klienci[i].pid==pid ){
			return (i+32);
		}
		
		else --puste_miejsca; 
			
	}

	if( puste_miejsca>0 ){
		getFifoName(pid, name);
		if(!STOP)
			printf("\nNazwa kolejki klienta: %s, ktory otrzyma numer: %i", name, index_pustego);

		klienci[index_pustego].pid= pid;
		sprintf(klienci[index_pustego].fifo_name, name);
		klienci[index_pustego].fp=NULL;
		klienci[index_pustego].cb= -1;
		return index_pustego;
	}

	return -1;
}

int fifoSK_init( int nr ){
	
	//unlink(name); 
	//printf("\nProba stworzenia kolejki: %s, dla klienta numer: %i",klienci[nr].fifo_name, nr);
	if ( mknod(klienci[nr].fifo_name, S_IFIFO | PRAWA, 0) < 0 ){
		printf("\nBlad tworzenia kolejki fifo do klienta %d", klienci[nr].pid);
		return -1;
	}

	//return fopen(klienci[nr].fifo_name, "wb");
	return 0;

}


void sendResults(int nr, char rs[]){

	//printf("\n\nnr klienta: %i", nr);
	Query q;
	q.PID=0;
	sprintf(q.Msg,"%s", rs);
	printQuery("\nOdsyłam klientowi nr:\t",q);
	fwrite( &q, sizeof(Query), 1, klienci[nr].fp  );
}

void remove_client(int nr){

	printf("\nUsuwam %i %s", nr, klienci[nr].fifo_name);	
	//fclose( klienci[nr].fp );
	unlink(klienci[nr].fifo_name);

	klienci[nr].pid=0;
	klienci[nr].fifo_name[0]='\0';
	klienci[nr].fp=NULL;

}

void *servant(int nr){

	char result[Q_SIZE]; 

	while( watki[nr].stan!=FREE ){
		printf("\ntu %d watek\n", nr);
		sprintf(result,"\nTu wątek %i", nr);
	
		if( strcmp(klienci[nr].Msg, MOFF)!=0 ){ 

				klienci[nr].fp= fopen(klienci[nr].fifo_name, "wb"); 
	
				if( klienci[nr].fp==NULL ) 
					printf("\nBlad otwarcia kolejki serwer-klient %i", nr);
				
				rozwal( klienci[nr].p, klienci[nr].Msg ); //printf("\n\n\n!!!!!!!!!!!!!!!!%s\n", klienci[nr].p[11].slowo);
				launch( klienci[nr].p, &klienci[nr].cb, result);
				sendResults(nr, result);
				cleanUpPol( klienci[nr].p );
				klienci[nr].Msg[0]='\0';
				watki[nr].stan=AWAITING;
		}
		else{ //printf("\nrządanie zakonczenia watku %i", nr);
				remove_client(nr);
				watki[nr].stan=FINISHED; 
				return (0);
		}	
		fclose( klienci[nr].fp );

		while(watki[nr].stan==AWAITING);
	}

	return (0);
}

void cleanUpPol(polecenie pol[MAXQSIZE]){

	int i;

	for(i=0; i<MAXQSIZE; ++i){
		pol[i].t=PNULL;
		pol[i].slowo[0]='\0';
	}

}
