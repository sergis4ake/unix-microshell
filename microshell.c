/*
    microshell.c
*/

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "microshell.h"
#include "comandos.h"
#include "auxiliares.h"

// Función para crear un número a partir de caracteres,
// utilizada para poder usar la sentencia <switch>.
const unsigned long hash(const char *strng) {
    unsigned long hash = 81;
    int i;

    while ((i = *strng++))
        hash = ((hash << 3) + hash) + i;
    return hash;
}

int main ()
{
    orden O;
    int r;
    char * shelldir, *shelldir2;
    									// Guardamos el directorio de la shell
    shelldir = working_directory();		// para poderlo utilizar en la orden <cd>
    
    
    printf ("Introduzca órdenes (pulse Ctrl-D para terminar)\n");

    do              // Leer órdenes y mostrarlas
    {
        inicializar_orden (&O);
		
		shelldir2 = working_directory();	// Obtenemos el directorio actual, 
											// para poder mostrarlo en el prompt.
        printf ("%s$ ", shelldir2);
        r = leer_orden (&O, stdin);
        
        if(O.argc == 0)		// Si no se introduce ninguna orden.
        	continue;
        else if (r < 0)
            fprintf (stderr, "\n[ERROR] %d: %s\n", -r, mensajes_err[-r]);
        else
            mostrar_orden (&O, shelldir);
		
        liberar_orden (&O);
    }
    while (r==0);   // Repetir hasta error o EOF

    return 0;
}

void mostrar_orden (orden * O, char *shelldir)
{  
    char *command = O->argv[0];
    switch(hash(command))
    {
    	case PWD:
    		mipwd(O);
    		break;
    		
    	case LS:
    		mils(O);
    		break;
    
    	case MKDIR:
    		mimkdir(O);
    		break;
    		
    	case RMDIR:
    		mirmdir(O);
    		break;
    	
    	case CD:
    		micd(O, shelldir);
    		break;
    		
    	case CAT:
    		micat(O);
    		break;
    	
    	case CP:
    		micp(O);
    		break;
    		
    	case RM:
    		mirm(O);
    		break;
    		
    	case EXIT:
    		exit(0);
    		
    	default:
    		printf("[ERROR] %s\n", mensajes_err[-ERROR_PARAMETROS_MAL]);
    }
}
