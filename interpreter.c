#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "filehnd.h"
#include "interpreter.h"


void launch( polecenie p[MAXQSIZE], int *currBase, char *text ){

	int stan, pom, pom2, pomk, i;
	char sklej[NKOL][NSIZE];

	if( p[0].t > DROP_DATABASE  &&  *currBase < 0){
		
		sprintf(text, "Najpierw wybierz baze, ktora chcesz uzywac.");
		return;
	}
	
	text[0]='\0';

	switch(p[0].t){

	case CREATE_DATABASE:

		stan= add_database(p[1].slowo);

		if( stan >= 0 ){
			sprintf(text, "Dodano baze %s", p[1].slowo);
		}
		else if( stan==-1 ){			
			sprintf(text, "Nie mozna juz dodac wiecej baz, osiagnieto maksimum");
		}
		else{
			sprintf(text, "%s - Baza o tej nazwie juz istnieje w systemie", p[1].slowo);
		}
		break;

	case DROP_DATABASE:
		
		if( ( pom= get_db(p[1].slowo) )== -1 ){
			sprintf(text, "%s - nie ma bazy o tej nazwie", p[1].slowo);
		}
		else{
			del_database(pom);
			sprintf(text, "Usunieto baze %s", p[1].slowo);
		}
		break;
	
	case USE:
		if( ( pom= get_db(p[1].slowo) )== -1 ){
			sprintf(text, "%s - nie ma bazy o tej nazwie", p[1].slowo);
		}
		else{
			*currBase=pom;
			sprintf(text, "Uzywasz bazy %s", p[1].slowo);
		}
		break;

	case SHOW_DATABASE:
		pri_database(text);
		break;

	case CREATE_TABLE:
		if( get_table(*currBase, p[1].slowo) > -1 ){
			sprintf(text, "%s - taka tabela już istnieje w biezacej bazie", p[1].slowo);
		}
		else{
			for( pom=2; pom<10; ++pom){ 
				if( p[pom].t==PNULL ) break;

				else
					sprintf(sklej[pom-2], p[pom].slowo); //printf("\n%d  %s", pom-2, sklej[pom-2]); }
			} 
			pom-=2; 

			stan= add_table(*currBase, p[1].slowo, sklej, pom);
			
			if( stan==-1){
				sprintf(text, "Nie mozna juz dodac wiecej tabel do tej bazy, osiagnieto maximum");
			}
			else if( stan==-2 ){
				sprintf(text, "Tabela liczy za dużo kolumn, maksimum to 7 (klucz glowny jest dodawany automatycznie)");
			}
			else
				sprintf(text, "Dodano tabele %s", p[1].slowo);
		}
		break;

	case DROP_TABLE:
		if( ( pom= get_table(*currBase, p[1].slowo) ) == -1 ){
			sprintf(text, "%s - taka tabela nie istnieje w biezacej bazie", p[1].slowo);
		}
		else{ printf("\nUsuwam %d",  pom);
			del_table(*currBase, pom);
			sprintf(text, "Usunieto tabele %s", p[1].slowo);
		}
		break;

	case SHOW_TABLE:
		pri_table(*currBase, text);
		break;

	case INSERT:
	case UPDATE: 
		if( ( pom2= get_table(*currBase, p[1].slowo) )== -1 ){
			sprintf(text, "%s - taka tabela nie istnieje w biezacej bazie", p[1].slowo);
		}
		else{
			for( pom=2; pom<10; ++pom){ 
				if( p[pom].t==PNULL ) break;

				else
					sprintf(sklej[pom-2], p[pom].slowo); //printf("\n%d  %s", pom-2, sklej[pom-2]); }
			} 
			pom-=2; 

			if(p[0].t==INSERT){
				stan= add_record(*currBase, pom2, sklej, pom); 
			}
			else{ printf("\n!!!!!!!!!!robię update");
				if(p[10].slowo[0]=='\0'){ // nie istnieje warunek WHERE
					sprintf(text, "Brak klauzuli WHERE");printf("brak where");
					break;
				}
				else{
					if ( ( pomk= get_column(*currBase, pom2, p[10].slowo) ) == -1 ){
						sprintf(text, "%s - taka kolumna nie istnieje w biezacej tabeli", p[10].slowo);
						
						break;
					}
					else{ printf("Znaleziono pasujący wzorzec w: baza: %i \ttabela: %i \tkolumna: %i", *currBase, pom2, pomk);
						for(i=0; i < baza.nrekord[*currBase][pom2]; ++i){
							printf("wpis w bazie: %s,  szukany: %s",baza.bd[*currBase][pom2][pomk][i],p[11].slowo);
							if( !strcmp( baza.bd[*currBase][pom2][pomk][i], p[11].slowo) ){
								stan= upd_record(*currBase, pom2, i, sklej, pom); printf("znaleziono pasujące w rek: %i",i);}
						}
					}	
				}
			}
			
			if( stan==-1){
				sprintf(text, "Nie mozna juz dodac wiecej rekordów do tej tabeli, osiagnieto maximum");
			}
			else if( stan==-2 ){
				sprintf(text, "%d Podales za duzo wartosci kolumn!", pom);
			}
			else if( stan==-3 ){
				sprintf(text, "%d Podales za malo wartosci kolumn!", pom);
			}
			else
				sprintf(text, "Nowy rekord w tabeli %s", p[1].slowo);
		}
		break;

	case SELECT:
		if( ( pom2= get_table(*currBase, p[1].slowo) )== -1 ){
			sprintf(text, "%s - taka tabela nie istnieje w biezacej bazie", p[1].slowo);
		}
		else{

			if(p[10].slowo[0]!='\0'){ // istnieje warunek WHERE
				
				if ( ( pomk= get_column(*currBase, pom2, p[10].slowo) ) == -1 ){
					sprintf(text, "%s - taka kolumna nie istnieje w biezacej tabeli", p[10].slowo);
					break;
				}	
			}
			else{
				pomk= -1; // będzie to znaczyć, że nie ma WHERE
			}

			if(p[2].slowo[0]=='*'){ // czyli wypisujemy wszystkie kolumny
				
				for(pom=0; pom < baza.nkolumn[*currBase][pom2]; ++pom)
					pri_nagl(*currBase, pom2, pom, text);

				pom= -1;
			}
			else{
				if ( ( pom= get_column(*currBase, pom2, p[2].slowo) ) == -1 ){
					sprintf(text, "%s - taka kolumna nie istnieje w biezacej tabeli", p[2].slowo);
					break;
				}
				else{
					pri_nagl(*currBase, pom2, pom, text);
				}
			}
			
			mark_up(text);

			for(i=0; i<baza.nrekord[*currBase][pom2]; ++i)
				pri_record(*currBase, pom2, i, pom, pomk, p[11].slowo, text, (pom<0) ? 0:1);
		}
		break;

	case DELETE:
		if( ( pom2= get_table(*currBase, p[1].slowo) )== -1 ){
			sprintf(text, "%s - taka tabela nie istnieje w biezacej bazie", p[1].slowo);
		}
		else{

			if(p[10].slowo[0]!='\0'){ // istnieje warunek WHERE
				printf("\np[10] niepuste");
				if ( ( pomk= get_column(*currBase, pom2, p[10].slowo) ) == -1 ){
					sprintf(text, "%s - taka kolumna nie istnieje w biezacej tabeli", p[10].slowo);
					break;
				}	
			}
			else{
				pomk= -1; // będzie to znaczyć, że nie ma WHERE
			}
			rem_record(*currBase, pom2, pomk, p[11].slowo);
			sprintf(text, "Wykonano DELETE");
		}
		break;

	default:
		sprintf(text, "%s - nieobslugiwane polecenie sql", p[0].slowo);
		break;

	}

	return;

}


int rozwal(polecenie p[MAXQSIZE], char *z){

    char *t;
    char t1[32];
    int i=0, j, k=0, l=0, m, a=0, b=0;
    
    //max 32 slowa w poleceniu

    polecenie tmp[MAXQSIZE];
    //zerowanie struktur
    for(i=0;i<32;i++){
        sprintf(tmp[i].slowo, "x");
        p[i].t=PNULL;
    }
    printf("%s\n", z);
    //max 128 znakow w poleceniu
    char s[128];
    sprintf(s, "%s", z);
    strtoupper(s, s, 128);
    printf("%s\n", s);
    
    
    t=strtok(s, " (),;=");
    while(t!=NULL) {
        k=strlen(t);
        for(j=0;j<k;j++) {
            p[l].slowo[j]=*t;
            *t++;
            //printf("%i = %i\n",i,j);
        }
        p[l].slowo[j]=0;
        p[l].t=PNULL;
        //printf("||%s||\n",p[i].slowo);
        t=strtok(NULL, " ()=,;");
        l++;
    }
    i=0;
    
    printf("%i\n", l);
    printf("0. %s", p[0].slowo);
    printf(" 1. %s", p[1].slowo);
    printf(" 2. %s", p[2].slowo);
    printf(" 3. %s", p[3].slowo);
    printf(" 4. %s", p[4].slowo);
    printf(" 5. %s", p[5].slowo);
    printf(" 6. %s", p[6].slowo);
    printf(" 7. %s", p[7].slowo);
    printf(" 8. %s", p[8].slowo);
    printf(" 9. %s", p[9].slowo);
    printf(" 10. %s", p[10].slowo);
    printf(" 11. %s\n", p[15].slowo);
    
    //w przypadku kiedy polecenie nie zaczyna sie od okreslonego slowa zwracamy blad
    if(!strcmp(p[0].slowo, "SELECT") || !strcmp(p[0].slowo, "INSERT") || !strcmp(p[0].slowo, "CREATE") || !strcmp(p[0].slowo, "DELETE") || !strcmp(p[0].slowo, "UPDATE") || !strcmp(p[0].slowo, "USE") || !strcmp(p[0].slowo, "DROP") || !strcmp(p[0].slowo, "SHOW")) {
        if(!strcmp(p[0].slowo, "SELECT")) p[0].t=SELECT;
        if(!strcmp(p[0].slowo, "INSERT")) p[0].t=INSERT;
        if(!strcmp(p[0].slowo, "CREATE")) {
            if(!strcmp(p[1].slowo, "TABLE")) p[0].t=CREATE_TABLE;
            if(!strcmp(p[1].slowo, "DATABASE")) p[0].t=CREATE_DATABASE;
            i++;
        }
        if(!strcmp(p[0].slowo, "DELETE")) p[0].t=DELETE;
        if(!strcmp(p[0].slowo, "UPDATE")) p[0].t=UPDATE;
        if(!strcmp(p[0].slowo, "DROP")) {
            if(!strcmp(p[1].slowo, "TABLE")) p[0].t=DROP_TABLE;
            if(!strcmp(p[1].slowo, "DATABASE")) p[0].t=DROP_DATABASE;
            i++;
        }
        if(!strcmp(p[0].slowo, "USE")) p[0].t=USE;
        if(!strcmp(p[0].slowo, "SHOW")) {
            if(!strcmp(p[1].slowo, "DATABASE")) p[0].t=SHOW_DATABASE;
            if(!strcmp(p[1].slowo, "TABLE")) p[0].t=SHOW_TABLE;
            i++;
        }
    }
    
    printf("%i\n", i);
    printf("0. %s", p[0].slowo);
    printf(" 1. %s", p[1].slowo);
    printf(" 2. %s", p[2].slowo);
    printf(" 3. %s", p[3].slowo);
    printf(" 4. %s", p[4].slowo);
    printf(" 5. %s", p[5].slowo);
    printf(" 6. %s", p[6].slowo);
    printf(" 7. %s", p[7].slowo);
    printf(" 8. %s", p[8].slowo);
    printf(" 9. %s", p[9].slowo);
    printf(" 10. %s", p[10].slowo);
    printf(" 11. %s\n", p[11].slowo);
    
    switch (p[0].t) {
        case SELECT:
            sprintf(tmp[1].slowo, "%s", p[3].slowo);
            sprintf(tmp[0].slowo, "");
            a=0;
            //znajdz z jakiej tabeli masz korzystac slowko po FROM
            do {
                a++;
                sprintf(tmp[1].slowo, "%s", p[a-1].slowo);
            }while(strcmp(p[a].slowo, "FROM"));
            printf("a: %i\n", a);
            //skopiuj wszystkie nazwy rekordow ktore maja byc uzyte
            do{
                b++;
                printf("B: %i/%s\n", b, p[b].slowo);
                sprintf(tmp[b+1].slowo, "%s", p[b+2].slowo);
            }while(strcmp(p[b+3].slowo, "WHERE") && b!=8);
            sprintf(tmp[b+2].slowo, "\0");
            tmp[b+2].t=PNULL;
            if(!strcmp(p[l-2].slowo, "WHERE")) {
                sprintf(tmp[10].slowo, "%s", p[l-2].slowo);
                sprintf(tmp[11].slowo, "%s", p[l-1].slowo);
            }

            char tmp1[32];
	    sprintf(tmp1,"%s",tmp[1].slowo);
	    sprintf(tmp[1].slowo,"%s",tmp[2].slowo);
	    sprintf(tmp[2].slowo,"%s",tmp1);
	    break;
        case INSERT:
            b=0;
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            do{
                b++;
                printf("B: %i/%s\n", b, p[b].slowo);
                sprintf(tmp[b+1].slowo, "%s", p[b+2].slowo);
		if(!strcmp(p[b+2].slowo,"")) tmp[b+1].t=PNULL;
            }while(b!=8);
            break;
        case DELETE:
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            sprintf(tmp[0].slowo, "");
            sprintf(tmp[2].slowo, "\0");
            if(!strcmp(p[l-3].slowo, "WHERE")) {
                sprintf(tmp[10].slowo, "%s", p[l-2].slowo);
                sprintf(tmp[11].slowo, "%s", p[l-1].slowo);
            }
            break;
        case UPDATE:
            i=0;
            sprintf(tmp[1].slowo, "%s", p[1].slowo);
            sprintf(tmp[0].slowo, "");
            do{
                b++;
                printf("B: %i/%s\n", b, p[b].slowo);
                sprintf(tmp[b+1].slowo, "%s", p[b+2].slowo);
            }while(strcmp(p[b+3].slowo, "WHERE") && b!=8);
            
            if(!strcmp(p[l-3].slowo, "WHERE")) {
                sprintf(tmp[10].slowo, "%s", p[l-2].slowo);
                sprintf(tmp[11].slowo, "%s", p[l-1].slowo);
                i=1;
            }
            if(i==1){
                if(l>11){
                    sprintf(tmp[9].slowo, "\0");
                }else{
                    sprintf(tmp[l].slowo, "\0");
                }
            }else{
                sprintf(tmp[l-1].slowo, "\0");
            }
            break;
        case USE:
            sprintf(tmp[1].slowo, "%s", p[1].slowo);
            sprintf(tmp[0].slowo, "\0");
            sprintf(tmp[2].slowo, "\0");
            break;
        case CREATE_TABLE:
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            for(a=3;a<l+1;a++){
                sprintf(tmp[a-1].slowo, "%s", p[a].slowo);
		if(!strcmp(p[a].slowo,"")) tmp[a-1].t=PNULL;
                if(a==10) break;
            }

            
            break;
        case CREATE_DATABASE:
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            break;
        case DROP_TABLE:
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            break;
        case DROP_DATABASE:
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            break;
        case SHOW_TABLE:
            sprintf(tmp[1].slowo, "%s", p[2].slowo);
            break;
        case SHOW_DATABASE:
            sprintf(tmp[1].slowo, "\0");
            sprintf(tmp[2].slowo, "\0");
            break;
    }
    for(i=1;i<12;i++){ // t - kopiujemy dopiero od i=1, a p[0].slowo i tak nie jest interpretowane
        sprintf(p[i].slowo, "%s", tmp[i].slowo);
        p[i].t=tmp[i].t;
        if(!strcmp(tmp[i].slowo,"x")) p[i].t=PNULL;
    }
    for(i=0;i<12;i++){
		switch(tmp[i].t){
			case PNULL:
				printf("Jest pnull na pozycji %i\n",i);
				break;
			default:
				printf("Brak nulla\n");
				break;
		}
	}
    //printf("%i\n", i);
    printf("0. %s", tmp[0].slowo);
    printf(" 1. %s", tmp[1].slowo);
    printf(" 2. %s", tmp[2].slowo);
    printf(" 3. %s", tmp[3].slowo);
    printf(" 4. %s", tmp[4].slowo);
    printf(" 5. %s", tmp[5].slowo);
    printf(" 6. %s", tmp[6].slowo);
    printf(" 7. %s", tmp[7].slowo);
    printf(" 8. %s", tmp[8].slowo);
    printf(" 9. %s", tmp[9].slowo);
    printf(" 10. %s", tmp[10].slowo); 
    printf(" 11. %s\n", tmp[11].slowo);
    
}

char *strtoupper(char *dest, const char *src, size_t n) {
    if (!n) {
        return 0;
    } else {
        char *d = dest;
        while (*src && --n>0) *d++ = toupper(*src++);
        *d = 0;
        return dest;
    }
}
