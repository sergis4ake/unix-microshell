/*
    microshell.h
*/

#include <stdio.h>

#ifndef _MICROSHELL_H_
#define _MICROSHELL_H_

// Declarando los valores de la funcion hash
// para los comandos internos.
#define PWD 69292
#define LS 7648
#define MKDIR 5585280
#define RMDIR 5619543
#define CD 7552
#define CAT 68057
#define CP 7564
#define RM 7696
#define EXIT 615851

void mostrar_orden (orden * O, char *shelldir);
extern const unsigned long hash(const char *strng);

#endif // _MICROSHELL_H_
