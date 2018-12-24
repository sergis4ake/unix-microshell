/*
	comandos.c
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

#include "parser.h"
#include "comandos.h"
#include "auxiliares.h"


void mipwd(orden *O)
{
	char *wdirectory;
	
	// Ruta absoluta del directorio actual.
	if( (wdirectory = working_directory()) == 0 ){
		perror("[ERROR] No se puede acceder a la ruta del directorio actual.");
		return;
	}
	printf("%s\n", wdirectory);
}

void mils(orden *O)
{
	char *wdir;		// Directorio actual.
	char *altdir;	// Directorio usado.
	DIR *dir;
	struct dirent *diren;
	int i = 0;
	char car;
	char *pathname = NULL;	// Reservamos memoria para el pathname.
	
	if ( (pathname = (char *) malloc(PATH_MAX)) == NULL)
		perror("[ERROR] No se puede reservar memoria con <malloc>");

	
	// Ruta absoluta del directorio actual.
	if( (wdir = working_directory()) == 0 ){
		perror("[ERROR] No se puede acceder a la ruta del directorio actual.");
		return;
	}
	
	// Asgnando en <altdir> el directorio con el que vamos a trabajar.
	switch (O->argc)
	{	
		// Si sólo hemos introducido <ls> en la consola, utilizamos el directorio actual.
		case 1:
			altdir = wdir;
			break;
			
		// Si hemos introducido <ls> y otro argumento en la consola.
		case 2:
		
			// Si el 2º argumento es <-l>, utilizamos el directorio actual.
			if( strcmp(O->argv[1], "-l") == 0 )
				altdir = wdir;
			
			// Si el 2º argumento de <ls> es el directorio y no <-l>, guardamos el directorio.
			else
				altdir = O->argv[1];
			break;
			
		case 3:		// Si hemos introducido <ls> y otros dos argumentos en la consola
						// significa que estamos utilizando un directorio diferente al acutal.
			altdir = O->argv[2];
			break;
			
		default:
			perror("[ERROR] Argumentos incorrectos para <ls>");
			return;
	}
	
	// Obtenemos el descriptor del directorio.
	if ( (dir = opendir(altdir)) == NULL ){
		perror("[ERROR] No se puede abrir el directorio.");
		return;
	}
	
	// Bucle para recorrer la estructura diren e ir mostrando la información.	
	while ( (diren = readdir(dir)) != NULL )
	{
		// No mostramos los archivos ocultos.
		car = diren->d_name[0];
		if( car != '.' )
		{
			
			// Leemos las entradas del directorio con todo detalle de cada archivo. [OPCIÓN -l]
			if( (O->argc > 1) && (strcmp(O->argv[1], "-l") == 0) )
			{
																	// Inicializamos el puntero del pathname,
				memset(pathname, 0, PATH_MAX);			// para poder volverlo a usar.
				
				
				if( (strlen(altdir) + strlen(diren->d_name) + 1) > PATH_MAX){
					perror("[ERROR] La ruta del directorio es demasiado grande.");
					return;
				}
				
				// Concatenamos el directorio que estamos utilizando con "/" y cada nombre de entrada.
				strcat(strcat(strcat(pathname, altdir), "/"), diren->d_name);
				
				colour_name(diren->d_type, diren->d_name);		// Asignamos un color al nombre de
																				// las entradas del directorio.
				lsdetailed(pathname);	// Función que muestra <ls -l> detallado.
				
			}
			// Leemos solo el nombre de las entradas del directorio. [OPCIÓN SIN -l]
			else if( (O->argc == 1) || (strcmp(O->argv[1], "-l") != 0) )
			{
				
				colour_name(diren->d_type, diren->d_name);	// Asignamos un color al nombre de
																			// las entradas del directorio.
				i++;
				if(i%5==0) printf("\n");
			}
		}
	}
	if ( (O->argc == 1) || (strcmp(O->argv[1], "-l") != 0) )
		printf("\n");
		
    // Liberamos memoria reservada del pathname.
    free(pathname);
 
	// Cerramos el directorio.
	if( (closedir(dir)) == -1 ){
		perror("[ERROR] No se puede cerrar el directorio.");
		return;
	}
}

void mimkdir(orden *O)
{	
	int i;
	
	// Comprobación de argumentos.
	if(O->argc == 1){
		perror("[ERROR] Argumentos incorrectos para <mkdir>.");
		return;
	}
	
	for(i = 1; i < O->argc; i++)
	{
		// Creamos el directorio con los permisos rwxrwxr-x.
		if( (mkdir(O->argv[i], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | 
							S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH) ) == -1 ){
			perror("[ERROR] No se puede crear el directorio especificado.");
			return;
		}
	}
}

void mirmdir(orden *O)
{
	int i;
	
	// Comprobación de argumentos.
	if(O->argc == 1){
		perror("[ERROR] Argumentos incorrectos para <rmdir>.");
		return;
	}
	
	for(i = 1; i < O->argc; i++)
	{
		// Borramos el directorio especificado.
		if( (rmdir(O->argv[i])) == -1 ){
			perror("[ERROR] No se puede borrar el directorio especificado.");
			return;
		}
	}
}

void micd(orden *O, char *shelldir)
{
	if(O->argc > 2){
		perror("[ERROR] Argumentos incorrectos para <cd>.");
		return;
	}
	if(O->argc == 1){
		if( (chdir(shelldir)) == -1 ){
			perror("[ERROR] No se puede acceder al directorio principal.");
			return;
		}
	}
	if(O->argc == 2){
		if( (chdir(O->argv[1])) == -1 ){
			perror("[ERROR] No se puede acceder al directorio especificado.");
			return;
		}
	}
}

void micat(orden *O)
{
	
	off_t sizefile;
	caddr_t p;
	char * archivo;
	int i, fd;
	// int sizepage;
	
	// Comprobación del número de argumentos.
	if(O->argc > 2 || O->argc < 2){
		perror("[ERROR] Argumentos incorrectos para <cat>.");
		return;
	}
	
	// Guardamos la ruta del archivo.
	archivo = O->argv[1];
	
	// Comprobación de la apertura del archivo.
	if( (fd = open (archivo, O_RDONLY)) == -1 ) {
		perror("[ERROR] No se puede abrir el archivo.");
		return;
	}
	
	// Utilizamos <lseek> para comprobar el tamaño del archivo. Posicionando el puntero 
	// al final del archivo, <lseek> nos devuelve el número de bytes desde el principio
	// de éste hasta la posición del puntero, es decir, el TAMAÑO del archivo.
	
	if ( (sizefile = lseek(fd, (off_t) 0, SEEK_END)) == -1 ) {
		perror("[ERROR] No se puede comprobar el tamaño del archivo con <lseek>.");
		return;
	}
	
	// Comprobamos el tamaño de página para poder acceder a un múltiplo. Ya que
	// el último parámetro de <mmap> es la posición del archivo desde la que se
	// va a realizar la proyección, y esta posicion debe ser un múltiplo del
	// tamaño de página. Pero como nuestro caso es el principio, será 0.
	
	//sizepage = sysconf(_SC_PAGE_SIZE);
	
	// Proyectando en memoria el archivo.
	if ( (void*)(p = mmap (NULL, (size_t) sizefile, PROT_READ, MAP_SHARED, fd, (off_t) 0)) == MAP_FAILED){
		perror("[ERROR] No se puede proyectar el archivo en memoria con <mmap>.");
		return;
	}
	
	// Recorremos el array que contiene los bytes proyectados en memoria.
	for(i=0;i<(int)sizefile;i++)
	{
		printf("%c", p[i]);
	}
	
	// Liberamos la memoria reservada y ocupada por la proyección del archivo.
	if( ( munmap(p, sizeof((size_t)sizefile)) ) == -1 ) {
		perror("[ERROR] No se puede liberar el espacio de memoria del archivo proyectado con <munmap>.");
		return;
	}
	
	// Cerrando el fichero.
	if( (close(fd)) == -1 ) {
		perror("[ERROR] No se puede cerrar el archivo.");
		return;
	}
	
}

void micp(orden *O)
{
	char car;
	off_t sizefile1, sizefile2;
	char *archivo1;
	char *archivo2;
	struct stat buffer;
	int posicion, fd1, fd2;
	
	// Comprobación del número de argumentos.
	if(O->argc > 3 || O->argc < 3){
		perror("[ERROR] Argumentos incorrectos para <cp>.");
		return;
	}
	
	// Guardamos la ruta del archivo1 y archivo2.
	archivo1 = O->argv[1];
	archivo2 = O->argv[2];
	
	// Comprobación de la apertura del archivo1.
	if( (fd1 = open (archivo1, O_RDONLY)) == -1 ) {
		perror("[ERROR] No se puede abrir el archivo1.");
		return;
	}
	
	// Comprobamos el tamaño del archivo1.
	if ( (sizefile1 = lseek(fd1, (off_t) 0, SEEK_END)) == -1 ) {
		perror("[ERROR] No se puede comprobar el tamaño del archivo con <lseek>.");
		return;
	}
	
	// Leemos las entradas de información de un archivo1, para poder
	// copiar los permisos del archivo1 al archivo2.
	if ( (stat(archivo1, &buffer)) == -1 )
	{
		perror("[ERROR] No se puede leer el estado del archivo.");
		return;
	}
	
	// Creamos el archivo2 con los persmisos del anterior.
	if( (fd2 = open (archivo2, O_WRONLY | O_CREAT | O_APPEND, buffer.st_mode)) == -1 ) {
		perror("[ERROR] No se puede abrir/crear el archivo2.");
		return;
	}
	
	// Comprobamos el tamaño del archivo2.
	if ( (sizefile2 = lseek(fd2, (off_t) 0, SEEK_END)) == -1 ) {
		perror("[ERROR] No se puede comprobar el tamaño del archivo con <lseek>.");
		return;
	}
	
	// Bucle para leer del primer archivo y añadir al segundo archivo.
	// En la poisción que pasamos como parámetro a EscribirCaracter,
	// será la misma posición que se lee del archivo1 más el tamaño
	// del archivo2. Es decir, posicionamos en relación del tamaño inicial
	// del archivo2, en relación de la posición final de antes de empezar cp.
	
	// No veía oportuno utilizar lseek con SEEK_END y posicionar al final del archivo, 
	// ya que cada iteración del bucle el final será el último caracter copiado.
	// Es decir, el final siempre será distinto en cada iteración del bucle.
	for (posicion=0; posicion<(int)sizefile1; posicion++)
	{
		// Guardamos en una variable el caracter leído del archivo1.
		car = LeerCaracter(fd1, posicion);
		
		// Escribimos el caracter leído del archivo1 dentro de archivo2.
		EscribirCaracter(fd2, posicion + (int)sizefile2, car);
	}
	
	// Cerrando el archivo1.
	if( (close(fd1)) == -1 ) {
		perror("[ERROR] No se puede cerrar el archivo a copiar.");
		return;
	}
	// Cerrando el archivo2.
	if( (close(fd2)) == -1 ) {
		perror("[ERROR] No se puede cerrar el archivo nuevo copiado.");
		return;
	}
	
}

void mirm(orden *O)
{
	int i;
	
	if(O->argc == 1){
		perror("[ERROR] Argumentos incorrectos para <rm>.");
		return;
	}
	
	for(i = 1; i < O->argc; i++)
	{
		// Borramos el directorio especificado.
		if( (unlink(O->argv[i])) == -1 ){
			perror("[ERROR] No se puede borrar el archivo especificado.");
			return;
		}
	}
}
//void exit(orden *O){}

