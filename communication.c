#include <string.h>
#include "communication.h"
#include <unistd.h>

char* getFifoName(int PID, char name[]){
	
	char pom[Q_SIZE];

	sprintf(pom, name);
	itoa(PID,pom,10);

	sprintf(name,"/tmp/"); 
	strcat(pom,".fifo"); 
	strcat(name,pom);
	//printf("\n%s",name); 

	return name;
}


void printQuery( char* when, Query t ){

	printf("\n%s", when);
	printf("%d\t%s\n", t.PID, t.Msg);
	
}

/** Ze wzgl�du na brak itoa() w standardzie C, u�y�em kodu 
	 tej funkcji oraz wyko�ystanej w niej strrev() napisanych przez: 
	 Elsayed'a Elghadban i Bob'a Stout **/

/*
**  ITOA.C - converts int to char*
**
**  public domain by Elsayed Elghadban
*/

char* itoa(int value, char*  str, int radix)
{
    int  rem = 0;
    int  pos = 0;
    char ch  = '!' ;
    do
    {
        rem    = value % radix ;
        value /= radix;
        if ( 16 == radix )
        {
            if( rem >= 10 && rem <= 15 )
            {
                switch( rem )
                {
                    case 10:
                        ch = 'a' ;
                        break;
                    case 11:
                        ch ='b' ;
                        break;
                    case 12:
                        ch = 'c' ;
                        break;
                    case 13:
                        ch ='d' ;
                        break;
                    case 14:
                        ch = 'e' ;
                        break;
                    case 15:
                        ch ='f' ;
                        break;
                }
            }
        }
        if( '!' == ch )
        {
            str[pos++] = (char) ( rem + 0x30 );
        }
        else
        {
            str[pos++] = ch ;
        }
    }while( value != 0 );
    str[pos] = '\0' ;
    return strrev(str);
}


/*
**  STRREV.C - reverse a string in place
**
**  public domain by Bob Stout
*/


char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}
