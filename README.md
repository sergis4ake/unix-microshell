# unix-microshell
Micro-shell que reconoce algunas órdenes internas de manejo de archivos y directorios.

Micro-shell es un intérprete de órdenes que solicita palabras clave (órdenes) por la entrada estándar y, en función de lo introducido, realiza unas tareas u otras.

## Objetivos
Las órdenes internas que interpreta son las siguientes:

* `pwd` muestra el directorio actual.
* `ls` y `ls -l` listado de archivos y directorios.
* `mkdir` crear nuevo directorio con el nombre pasado por parámetro.
* `rmdir` elimina un directorio pasado por parámetro.
* `cd` cambia el directorio actual.
* `cat` muestra el contenido del archivo que se le pase por parámetro.
* `cp` utilizando las funciones de POSIX `read()` y `write()`, copia el contenido del primer archivo pasado por parámetro en el archivo pasado como segundo parámetro.
* `rm` elimina el archivo que se le pase por parámetro, utilizando la funcion de POSIX `unlink()`.
* `exit` termina la ejecución de la microshell.
