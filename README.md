# RDBMS_From_Uni
My approach to create RDBMS, project I did with my friend Tomek Zajdel at Uni, when we both did not know much about coding.
Rest is in polish, as most likely will be accessed by students from my homeland.

# Proszę uważnie przeczytać

Projekt rozwiązuje następujący problem

Serwer + Klient bazy danych SQL’owej
1. Serwer wspólbiezny: watki

2. Komunikacja: kolejki fifo

3. Baza danych – wszystkie bazy danych, tabele i pliki indeksowe znajduja sie w
jednym pliku )

4. Akceptowalne polecenia sql’a (create, insert, delete, update, select) + blokady na
poziomie rekordów

5. Unix, jezyk C, tryb tekstowy (biblioteka curses)

Nie posiadam już dokumentacji do projektu i nie uruchamiałem go od 8 lat, eksperymenty pozostawiam wam. Składnia była bliska MySQL, trzeba założyć nową bazę, dodać tabelę, kilka wierszy i wykonać select, update. Na pewno wywali się na brdziej skomplikowanych zapytaniach ;)

Dołączyłem skompilowany server ( trzeba odpalić jako pierwszy ) i klienta, gdyby ktoś był na tyle leniwy, ale w paczce jest też makefile.

Projekt był napisany przez 2 studentów, którzy dopiero uczyli się programowania, na pewno zawiera sporo błędów i dzisiaj trochę się go wstydzimy, ale był wystarczająco dobry by zdać.

Nie odpowiadam na zapytania mailowe, dotyczące modyfikacji, lub napisania podobnego projektu. Rodzina i praca zajmują większość mojego czasu.

Proszę o zostawienie gwiazdki zalogowanych użytkowników w formie gratyfikacji.
