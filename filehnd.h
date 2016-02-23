#ifndef _MFILEHANDLER
#define _MFILEHANDLER

// maksymalne rozmiary poszczególnych elementów w bazie
#define NBAZ	4
#define NTAB	8
#define NREC	32
#define NKOL	8
#define NSIZE	32

#define FBASE "baza.6r"

// cała baza zajmuje w pamięci <300 kB

typedef struct{
		
		/** NAGŁÓWEK **/
		// nazwy poszczególnych elementów w pliku		
		char sbazy[NBAZ][NSIZE];
		char stabele[NBAZ*NTAB][NSIZE];
		char skolumn[NBAZ*NTAB*NREC][NSIZE];
		// uwaga założyliśmy, że rekordy nie potrzebują identyfikatora		

		// ilość poszczególnych elementów w pliku - bardzo ułatwi nawigację
		short nbazy;						// ile baz jest aktualnie w pliku
		short ntabele[NBAZ];			// ile tabel jest w bazie o danym numerze
		short nkolumn[NBAZ][NTAB];	// ile kolumn jest w bazie i tabeli o danych numerach
		short nrekord[NBAZ][NTAB]; 	// ile rekordów jest w bazie i tabeli o danych numerach

		/** AKTUALNIE BLOKOWANE REKORDY **/

		//short lock[NBAZ][NTAB][NREC]; // tabela ma zawierać same 0 (nieblokowane) i 1 (blokowane)
		
		/** ZAWARTOŚĆ PLIKU Z BAZĄ DANYCH **/
		char bd [NBAZ][NTAB][NKOL][NREC][NSIZE];	// tu wczytujemy cały plik z bazą
}Baza;

Baza baza; 


/***		
		To są funkcje, które napiszę i będziesz je mógł użyć do obsługi zapytań klienta
		Standardowo: 
			w funkcjach zwracających int: 0 - sukces, <0 porażka ( ewentualnie kod błędu )
			w funkcjach zwracających char* - char[0]='\0' - porażka, wszystko inne to sukces
			w funkcji zwracającej char* - char[0][0]='\0' - porażka, wszystko inne to sukcess

		Ty ich po prostu używaj, tym jak będą działać ja się zajmę ;)
***/
			
/*------------------- FUNKCJE ODNOSZĄCE SIĘ DO BAZ DANYCH W PLIKU --------------------------*/

// tworzy bazę danych o podanej nazwie zwraca jej nr w strukturze, 
// lub -1 - jeżeli nie da się dodać żadnej więcej bazy, -2 - taka baza już istnieje
int add_database(char name[]); 

// usuwa bazę danych o podanej nazwie, zwraca 0 - jeżeli ok, -1 jeśli nie ma takiej bazy
int del_database(int ndb);

// zwraca nr bazy o nazwie "name" ze struktury, -1 jak nie ma takiej bazy
int get_db(char name[]);

// wpisuje do argumentu char* nazwy wszystkich baz danych ze struktury
// poszczególne nazwy są oddzielone '\n' np: "Firma\nAdministracja\nZUS"
// służy tylko do pokazania baz danych na ekranie !
void pri_database(char*);


/*------------------- FUNKCJE ODNOSZĄCE SIĘ DO TABEL W PLIKU --------------------------*/

// dodaje nową tabelę o nazwie name do bazy o numerze ndb ze struktury,
// dodatkowo tworzy strukturę tabeli na podstawie nazw kolumn podanych jako
// atrybut *kolumny[]. Liczba kolumn musi byż z góry określona za pomocą atrybutu skol.
// Przykład użycia:  
// 
// char *kol[3]={ "Id", "Imię", "Nazwisko" };
// add_table(0,"Klienci", kol, 3);
// zwraca 0 - jeśli udało się utworzyć nową tabelę,
// -1 nie można już dodać żadnej nowej tabeli
// -2 taka tabela już istnieje
// -3 za dużo kolumn 
int add_table(int ndb, char name[], char kolumny[][NSIZE], int skol);

// usuwa tabelę o nazwie name z bazy o numerze ndb ze struktury, zwraca 0 - jeżeli ok, -1 jeśli się nie udało
int del_table( int ndb, int ntab );

// zwraca nr tabeli o nazwie "name" przypisanej do bazy o numerze ndb ze struktury, -1 jak nie ma takiej tabeli
int get_table(int ndb, char name[]);

// zwraca w tabeli char[] nazwy wszystkich tabel z bazy o numerze ndb ze struktury
// poszczególne nazwy są oddzielone '\n' np: "Klienci\nSprzedawcy\nDostawcy"
// służy tylko do pokazania tabel na ekranie !
void pri_table(int ndb, char *n);


/*------------------- FUNKCJE ODNOSZĄCE SIĘ DO REKORDÓW W PLIKU --------------------------*/

// wstawia nowy rekord do tabeli o numerze ntab z bazy o numerze ndb ze struktury 
// przykład użycia analogiczny jak w przypadku funkcji add_table()
// zwraca 0 - jeśli się udało, -1 - jeżeli nie da się już dodać nowych rekordów do tabeli
// -2 jeżeli ilość danych zbyt duża, -3 - ilość danych za mała,
// W zamierzeniu ma obsługiwać polecenie INSERT
int add_record(int ndb, int ntab, char rekord[][NSIZE], int rlen);

// usuwa rekord o numerze nrec z tabeli o numerze ntab z bazy o numerze ndb ze struktury 
// ta funkcja powinna być wywołana przez inne funkcje kasujące:
// del_database(), del_table(), rem_record(), user bezpośrednio jej nie używa !
int del_record(int ndb, int ntab, int nrec);

// blokuje rekord o numerze nrec z tabeli o numerze ntab z bazy o numerze ndb ze struktury 
int blk_record(int ndb, int ntab, int nrec);

// zmienia rekord o numerze nrec z tabeli o numerze ntab z bazy o numerze ndb ze struktury 
// zwraca 0 - jeśli się udało, -1 jeżeli ilość danych zbyt duża, -2 - ilość
// danych za mała, -3 - jeżeli dany rekord nie istnieje
// W zamierzeniu ma obsługiwać polecenie UPDATE
int upd_record(int ndb, int ntab, int nrec, char rekord[][NSIZE], int rlen);

// usuwa wszystkie rekordy z tabeli ntab z bazy o numerze ndb, w których
// kolumna o numerze nkol zawiera wartość text
// funkcja obsługuje zapytania typu DELETE ? FROM ? WHERE ?
int rem_record(int ndb, int ntab, int nkol, char *text);

// Drukowanie wartości wybranych kolumn w rekordzie
// nl zależy od nrec, jak nrec=-1 - nl=0
void pri_record(int ndb, int ntab, int nrec, int nkol, int nkolspr, char *phr, char *n, int nl);

/*------------------- FUNKCJE ODNOSZĄCE SIĘ DO KOLUMN W PLIKU --------------------------*/

// zwraca nr kolumny o nazwie "name" z tabeli o numerze ntab, przypisanej do bazy o numerze ndb ze struktury
int get_column(int ndb, int ntab, char name[]);

// zwraca w tabeli char[] wszystkie wartości odczytane
// z kolumny o numerze nkol z tabeli o numerze ntab z bazy 
// o numerze ndb
// Ta funkcja ma służyć do obsługi polecenia SELECT
void pri_column(int ndb, int ntab, int nkol, char *phr, char *n);


// drukuje nagłówki kolumn, potrzebne do SELECT
// pzrzyjmuje takie same parametry jak pri_column()
void pri_nagl(int ndb, int ntab, int nkol, char *n);


/*------------------------------- POZOSTAŁE --------------------------------------------*/

void init_db(void); 		// jeżeli to pierwsze uruchomienie, zainicjuje strukturę systemu
int read_base(void);		// odczyt pliku z bazami danych
int write_base(void);	// zapis pliku
void mark_up(char*);		// robi podkreślenia pod nagłówkiem tabeli


#endif
