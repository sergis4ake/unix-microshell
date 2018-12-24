/*
    auxiliares.h
*/

#include <stdio.h>

#ifndef _AUXILIARES_H_
#define _AUXILIARES_H_

// Los colores que vamos a utilizar con los distintos 
// tipos de archivo al mostrar con ls.
#define RESET   "\033[0m"
#define BOLDBLACK   "\033[1m\033[30m"      // Bold Black
#define BOLDRED     "\033[1m\033[31m"      // Bold Red
#define BOLDGREEN   "\033[1m\033[32m"      // Bold Green
#define BOLDYELLOW  "\033[1m\033[33m"      // Bold Yellow
#define BOLDBLUE    "\033[1m\033[34m"      // Bold Blue
#define BOLDMAGENTA "\033[1m\033[35m"      // Bold Magenta
#define BOLDCYAN    "\033[1m\033[36m"      // Bold Cyan
#define BOLDWHITE   "\033[1m\033[37m"      // Bold White

// Funciones auxiliares.
char *working_directory();
void lsdetailed(char *d_name);
void colour_name(int d_type, char *d_name);
char LeerCaracter(int fd, int posicion);
void EscribirCaracter(int fd, int posicion, char car);

#endif // _AUXILIARES_H_
