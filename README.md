# Proyecto de C para prebercarios del Proteco

Proyecto final del curso de lenguaje C para prebecarios del PROTECO de la FI de la UNAM.

Controlador de versiones hecho en lenguaje C para sistemas GNU/Linux. Utiliza sólo una rama "master" y almacena todos los archivos en una carpeta oculta dentro del directorio donde se mande llamar el comando.

Correr estos comandos para compilar:
	gcc -c gitfuncs.c -o gitfuncs.o
	gcc -c git.c -o git.o
	gcc git.o gitfuncs.o -o git
	
Para ejecutar, escribir en consola el nombre del ejecutable seguido de el comando que se desea ejecutar. Los comandos son iguales a los del controlador de versiones Git.

