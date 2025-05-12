assembler: main.o structures.o checkers.o macro.o 
	gcc -g -ansi -Wall -pedantic -fsanitize=address main.o structures.o checkers.o macro.o -o assembler
main.o: main.c structures.h checkers.h macro.h
	gcc -c -g -ansi -Wall -pedantic -fsanitize=address main.c -o main.o
structers.o: structures.c structures.h data.h 
	gcc -c -g -ansi -Wall -pedantic -fsanitize=address structures.c -o structures.o
checkers.o: checkers.c checkers.h data.h structures.h
	gcc -c -g -ansi -Wall -pedantic -fsanitize=address checkers.c -o checkers.o
macro.o: macro.c macro.h checkers.h data.h structures.h  
	gcc -c -g -ansi -Wall -pedantic -fsanitize=address macro.c -o macro.o

