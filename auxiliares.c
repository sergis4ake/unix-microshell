/*
    auxiliares.c
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "auxiliares.h"

// Función para comprobar el directorio actual <pwd>
char *working_directory()
{
	size_t size = 50;
	char * buffer = NULL;
	
	for(;;)
	{
		// Reservamos memoria del array buffer con <malloc()>
		if ( (buffer = (char *) malloc(size)) == NULL){
			perror("[ERROR] No se puede reservar memoria con <malloc>");
			return 0;
		}
		
		if (getcwd(buffer, size) == buffer)		// <getcwd()> devuelve un puntero a buffer si el path
			return buffer;								// cabe en el array reservado, sino devuelve cero.
		
		free(buffer);			// Liberamos la memoria reservada.
		
		if (errno != ERANGE)	// Si se produce cualquier otro error que no sea
		{							// por el tamaño del buffer, retornamos cero.
			return 0;
		}
		size *= 2;				// Incrementamos el tamaño del array reservado con <malloc()>
		
		if(size >= PATH_MAX)	// Si la ruta del directorio es demasiado larga.
			return 0;
	}
}

void colour_name(int d_type, char *d_name)
{
	// Dependiendo del tipo de archivo mostraremos el nombre del archivo de diferente color.
	switch(d_type)
	{
		// BLUE para directorios.
		case DT_DIR:
			printf(BOLDBLUE "%-20s" RESET, d_name);
			break;
		// RED para enlaces.
		case DT_LNK:
			printf(BOLDRED "%-20s" RESET, d_name);
			break;
		// GREEN para especial caracteres.
		case DT_CHR:
			printf(BOLDGREEN "%-20s" RESET, d_name);
			break;
		// WHITE negrita para especial bloques.
		case DT_BLK:
			printf(BOLDWHITE "%-20s" RESET, d_name);
			break;
		// WHITE normal para el resto.
		default:
			printf("%-20s", d_name);
			break;
	}
}

void lsdetailed(char *pathname)
{
	int i;
	struct stat buffer;
	char permissions [] = {'x', 'w', 'r'};
	char * date;
	struct passwd *pwd;
	struct group *grp;
	
	// Leemos las entradas de información de un archivo.
	if ( (stat(pathname, &buffer)) == -1 )
	{
		perror("[ERROR] No se puede leer el estado del archivo.");
		return;
	}
	
	// Permisos de lectura, escritura y ejecución.
	printf("0%-4o ", buffer.st_mode & 0777);
	for (i = 0; i < 9; i++)
	{
		if(buffer.st_mode & (0400 >> i))
			printf("%-1c", permissions[(8-i)%3]);
		else
			printf("-");
	}
	printf("\t");
	
	// UID
	//printf("%d ", buffer.st_uid);
	if( (pwd = getpwuid(buffer.st_uid)) != NULL)
		printf("%-4s  ", pwd->pw_name);
	else
		printf("???\t");
		
	// GID
	//printf("%d ", buffer.st_gid);
	if( (grp = getgrgid(buffer.st_gid)) != NULL)
		printf("%-4s  ", grp->gr_name);
	else
		printf("???\t");
	
	// Tamaño del fichero.
	printf("%-7d", (int)buffer.st_size);

	// Tipo de fichero.
	switch(buffer.st_mode & S_IFMT)
	{
		case S_IFREG:
			printf("ordinario\t");
			break;
		case S_IFDIR:
			printf("directorio\t");
			break;
		case S_IFCHR:
			printf("esp caracter\t");
			break;
		case S_IFBLK:
			printf("esp bloque\t");
			break;
		case S_IFIFO:
			printf("FIFO\t");
			break;
		default:
			printf("desconocido\t");
	}
	
	// Última modificación.
	
	date = asctime(localtime(&buffer.st_mtime));
	printf("%s", date);
	
}

/** Funciones recicladas del programa anterior para el comando <cat>. **/
char LeerCaracter(int fd, int posicion)
{
	char caracter;
	off_t p;
	
	// Posicionando el puntero en la posición pasada por parámetro.
	if ( (p = lseek(fd, (off_t) posicion, SEEK_SET)) == -1 ) {
		perror("[ERROR] No se puede posicionar el puntero en el archivo.");
		exit(1);
	}
	
	// Leyendo del archivo mediante la llamada al sistema <read> un solo byte.
	if( (p = read(fd, &caracter, sizeof(caracter))) == -1) {
		perror("[ERROR] No se puede leer el archivo.");
		exit(2);
	}
	
	return caracter;
}

void EscribirCaracter(int fd, int posicion, char car)
{
	off_t p;
	
	// Posicionando el puntero en la posición pasada por parámetro.
	if ( (p = lseek(fd, (off_t) posicion, SEEK_SET)) == -1 ) {
		perror("[ERROR] No se puede posicionar el puntero en el archivo.");
		return;
	}
	
	// Escribiendo en el archivo mediante la llamada al sistema <write> un solo byte.
	if( (p = write(fd, &car, sizeof(car))) == -1) {
		perror("[ERROR] No se puede escribir en el archivo.");
		return;
	}
}
