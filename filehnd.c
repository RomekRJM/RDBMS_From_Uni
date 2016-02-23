#include <stdio.h>
#include <string.h>
#include "filehnd.h"
#include "communication.h"

void init_db(void){	// działa dobrze

	int i,j,k,l,m=0,n=0;

	baza.nbazy=0;

	for(i=0; i<NBAZ; ++i){
		
		baza.ntabele[i]=0;	
		baza.sbazy[i][0]='\0';

		for(j=0; j<NTAB; ++j, ++n){

			baza.nkolumn[i][j]=0;	
			baza.nrekord[i][j]=0;	
			baza.stabele[n][0]='\0';
			
			for(k=0; k<NKOL; ++k, ++m){
				
				//baza.lock[i][j][k]=0;
				baza.skolumn[m][0]='\0';	
				
				for(l=0; l<NREC; ++l){

					baza.bd[i][j][k][l][0]='\0';						
				}
					
			}

		}

	}

}

int read_base(){	// działa, ale mało testowane

	FILE *fp;

	if ( ( fp= fopen(FBASE,"r") ) == NULL )
		return -1;

	fread(&baza, sizeof(baza), 1, fp);
	fclose(fp);

	return 0;
}

int write_base(){	// działa, ale mało testowane

	FILE *fp;

	if ( ( fp= fopen(FBASE,"w") ) == NULL )
		return -1;

	fwrite(&baza, sizeof(baza), 1, fp);
	fclose(fp);

	return 0;
}

int add_database(char name[]){	// działa dobrze

	int i;

	if(baza.nbazy > NBAZ-1) // nie można już dodać żadnej nowej bazy
		return -1;

	if( get_db(name) >= 0 ) // taka baza już istnieje
		return -2;

	for( i=0; i<NBAZ; ++i ){
		
		if ( baza.sbazy[i][0] == '\0' ){
			sprintf(&baza.sbazy[i], name);
			break;
		}
	}
	++baza.nbazy;
	//printf("nazwa %s", &baza.sbazy[i]);
	return i;	// udało się stworzyć bazę, zwracam jej numer

}	

int del_database(int ndb){	// działa, ale trzeba uzłupełnić o kasowanie tab, kol i rek.

	int i;

	for(i=0; i<baza.ntabele[ndb]; ++i){
		del_table(ndb, i);
	}

	baza.sbazy[ndb][0]='\0';
	--baza.nbazy;
	//printf("nazwa %s", &baza.sbazy[i]);
	return 0;	// udało się stworzyć bazę, zwracam jej numer

}	


int get_db(char name[]){	// działa dobrze

	int i;
	
	for( i=0; i<NBAZ; ++i ){
		
		if ( strcmp(&baza.sbazy[i], name)==0 ){
			return i;
		}
	}

	return -1;

}

void pri_database(char *n){	// działa dobrze
	
	int i,j;

	sprintf(n,"\nIstnieje %d baz danych w systemie: \n", baza.nbazy);

	for(i=0; i < NBAZ; ++i){
		
		if( baza.sbazy[i][0]!='\0'){
			strcat(n, "\n"); //printf("\n%d\n",i);
			strcat(n, &baza.sbazy[i] );
		}
	}

	//printf("\nW funkcji: %s",n);

}


int add_table(int ndb, char name[], char kolumny[][NSIZE], int skol){

	int i,j,tmp,tmp2;

	++skol;	// id dodajemy sobie sami


	if(baza.ntabele[ndb] > NBAZ-1) // nie można już dodać żadnej nowej tabeli
		return -1;

	if(skol > NKOL)	// za dużo kolumn ! 
		return -2;

	for( i=0; i<NTAB; ++i ){
		
		tmp= (ndb*NTAB) + i;
		if ( baza.stabele[tmp][0] == '\0' ){
			sprintf(&baza.stabele[tmp], name);

			for(j=0; j<skol; ++j){
			
				tmp2= (tmp*NKOL) + j;
	
				if(j==0) sprintf(&baza.skolumn[tmp2], "Id");
				else		sprintf(&baza.skolumn[tmp2], kolumny[j-1]);
			
			}
			break;
		}
	}
	baza.nkolumn[ndb][i]=skol;
	++baza.ntabele[ndb];
	//printf("\nnazwa %s\n", &baza.nkolumn[ndb][j]);
	return i;	// udało się stworzyć bazę, zwracam jej numer

}


int del_table( int ndb, int ntab ){

	int j,tmp,tmp2;
	
	for(j=0; j<baza.nrekord[ndb][ntab]; ++j){

		del_record( ndb, ntab, j ); // usunięcie po koleii wszystkich rekordów z tabeli
	}

	tmp= (ndb*NTAB) + ntab;
	baza.stabele[tmp][0]='\0';

	for(j=0; j<baza.nkolumn[ndb][ntab]; ++j){
			
		tmp2= (tmp*NKOL) + j;
		baza.skolumn[tmp2][0]='\0';
			
	}

	baza.nkolumn[ndb][ntab]=0;
	--baza.ntabele[ndb];

	return 0;	

}


int get_table(int ndb, char name[]){	// działa dobrze

	int i, tmp;
	
	for( i=0; i<NTAB; ++i ){
		
		tmp= (ndb*NTAB) + i;
		if ( strcmp(&baza.stabele[tmp], name)==0 ){
			return i;
		}
	}

	return -1;

}


void pri_table(int ndb, char *n){

	int i,j,tmp,tmp2;

	sprintf(n,"\nIstnieje %d tabel w bazie %s:\n", baza.ntabele[ndb], &baza.sbazy[ndb] );

	for(i=0; i < NTAB; ++i){
		
		tmp= (ndb*NTAB) + i;
		if( baza.stabele[tmp][0]!='\0'){
			strcat(n, "\n");
			strcat(n, &baza.stabele[tmp] );
			strcat(n, "\t( "); // dopisać wyświetlanie kolumn

			for(j=0; j<NKOL; ++j){
			
				tmp2= (tmp*NKOL) + j;
				if( baza.skolumn[tmp2][0]!='\0'){

					if( j>0 ) strcat(n, ", ");
					strcat(n, &baza.skolumn[tmp2] );
				}
			}
	
			strcat(n," )");
		}
	}

	//printf("\nW funkcji: %s",n);

}


int add_record(int ndb, int ntab, char rekord[][NSIZE], int rlen){	// sprawdzić to !

   int i,j;
	char  str[4];
	//printf("----------------- %d",baza.nkolumn[ndb][ntab]);
	++rlen;

	if(baza.nrekord[ndb][ntab] > NREC-1) // nie można już dodać żadnego nowego rekordu
		return -1;

	if(rlen > baza.nkolumn[ndb][ntab])	// za dużo kolumn ! 
		return -2;

	if(rlen < baza.nkolumn[ndb][ntab]) // za mało kolumn !
		return -3; 

	for( i=0; i<NREC; ++i ){		// 'i' - nr pierwszego pustego rekordu
		
		if( baza.bd[ndb][ntab][0][i][0] == '\0' )
			break;
	}

	itoa(i, str, 10);
	sprintf(&baza.bd[ndb][ntab][0][i], str );

	for(j=1; j<rlen; ++j){
		
		sprintf(&baza.bd[ndb][ntab][j][i], rekord[j-1]);
	}
	
	++baza.nrekord[ndb][ntab];
	//printf("\nDodałem rekord %s\n", &baza.nkolumn[ndb][j]);
	return i;	// udało się stworzyć bazę, zwracam jej numer	


}


int del_record(int ndb, int ntab, int nrec){	// źle

	int i;

	//printf("\nusuwam");
	for(i=0; i<baza.nkolumn[ndb][ntab]; ++i){
			
		baza.bd[ndb][ntab][i][nrec][0]='\0';
	}

	--baza.nrekord[ndb][ntab];

	return 0;
}

int rem_record(int ndb, int ntab, int nkol, char *text){	// źle

	int i,tmp;

	//printf("\n%d\n", baza.nrekord[ndb][ntab]);
	tmp= baza.nrekord[ndb][ntab];

	for(i=0; i<tmp; ++i){

		if( ( !strcmp( &baza.bd[ndb][ntab][nkol][i], text ) ) || (nkol < 0) )
			del_record(ndb, ntab, i); 
	}

}


int upd_record(int ndb, int ntab, int nrec, char rekord[][NSIZE], int rlen){	// dobrze, ale jeszcze przetestować

	int i;

	++rlen;
	
	if(rlen > baza.nkolumn[ndb][ntab])	// za dużo kolumn ! 
		return -2;

	if(rlen < baza.nkolumn[ndb][ntab]) // za mało kolumn !
		return -3; 

	for(i=1; i<rlen; ++i){
		sprintf(&baza.bd[ndb][ntab][i][nrec], rekord[i-1]);
	}

	return 0;

}


void pri_record(int ndb, int ntab, int nrec, int nkol, int nkolspr, char *phr, char *n, int nl){	// dobrze

	int i, ile;
	//n[0]='\0';
	
		if( nkol == -1 ){	// '*'
			for(i=0; i <baza.nkolumn[ndb][ntab]; ++i){
				pri_record(ndb, ntab, nrec, i, nkolspr, phr, n, nl);
			}
			if( n[ strlen(n)-1 ]=='|' )	strcat(n, "\n");	
		}
		else{ 
				//strcat(n, "\t");
			if( ( nkolspr>=0 ) && ( strcmp( phr, &baza.bd[ndb][ntab][nkolspr][nrec] ) ) )	return;
			else{
				strcat(n,"  ");
				strcat(n, &baza.bd[ndb][ntab][nkol][nrec] );
				ile=strlen(&baza.bd[ndb][ntab][nkol][nrec]) / 5;  
				
				if(ile<4){
					while(++ile < 3) { strcat(n,"\t"); }
					strcat(n, "|");
				}
				if(nl)	 strcat(n, "\n");
			}
		}
	
}


int get_column(int ndb, int ntab, char name[]){

	int i, tmp;

	for(i=0; i<baza.nkolumn[ndb][ntab]; ++i){

		tmp= ((ndb*NTAB) + ntab)*NKOL + i;
		if( strcmp(baza.skolumn[tmp], name) == 0 )	return i;
	}

	return -1;
}


void pri_column(int ndb, int ntab, int nkol, char *phr, char *n){

	int i;
	printf("\nphr= %s", phr);

	for(i=0; i<baza.nrekord[ndb][ntab]; ++i){

		if( !strcmp(phr, &baza.bd[ndb][ntab][nkol][i]) )
		{
			strcat(n,"\n");
			strcat(n, &baza.bd[ndb][ntab][nkol][i]);
		}
	}

}


void pri_nagl(int ndb, int ntab, int nkol, char *n){

	int tmp, ile;

	tmp= ((ndb*NTAB) + ntab)*NKOL + nkol;

	strcat(n, "  ");
	strcat(n, baza.skolumn[tmp]);
	
	ile=strlen(baza.skolumn[tmp]) / 5;  
		
	if(ile<4){
		while(++ile < 3) { strcat(n,"\t"); }
		strcat(n, "|");
	}

}

void mark_up(char* s){

	int i, cntr=0; // cntr=-6, po to aby nie liczyć "\t  " na początku

	for(i=0;  ; ++i){
		if(s[i]=='\0') break;
		else if(s[i]=='\t') cntr+=5;
		else ++cntr;
	}
	strcat(s,"\n");
	
	for(i=0;  i<=cntr; ++i){
		strcat(s,"-");
	}

	strcat(s,"\n");
}



