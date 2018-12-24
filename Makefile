all: microshell

microshell: microshell.o parser.o comandos.o auxiliares.o
	gcc -g -Wall -o microshell microshell.o parser.o comandos.o auxiliares.o

microshell.o: microshell.c microshell.h
	gcc -g -Wall -c -o microshell.o microshell.c

parser.o: parser.c parser.h
	gcc -g -Wall -c -o parser.o parser.c

comandos.o: comandos.c comandos.h
	gcc -g -Wall -c -o comandos.o comandos.c

auxiliares.o: auxiliares.c auxiliares.h
	gcc -g -Wall -c -o auxiliares.o auxiliares.c

clean:
	rm -f microshell.o parser.o comandos.o auxiliares.o microshell
