makeitall: klientfinal serverfinal clean

serverfinal: hlfifoserv.o communication.o interpreter.o filehnd.o
	gcc -lm hlfifoserv.o communication.o interpreter.o filehnd.o -o server -lpthread -lrt

hlfifoserv.o: hlfifoserv.c hlfifoserv.h communication.h interpreter.h
	gcc -Wall -c hlfifoserv.c -o hlfifoserv.o

interpreter.o: interpreter.c interpreter.h filehnd.h
	gcc -Wall -c interpreter.c -o interpreter.o

filehnd.o: filehnd.c filehnd.h communication.h
	gcc -Wall -c filehnd.c -o filehnd.o 

klientfinal: klient.o communication.o
	gcc -lm klient.o communication.o -o klient -lncurses

klient.o: klient.c  communication.h
	gcc -Wall -c klient.c -o klient.o

communication.o: communication.c communication.h
	gcc -Wall -c communication.c -o communication.o

.PHONY: clean

clean:
	rm -f *.o
