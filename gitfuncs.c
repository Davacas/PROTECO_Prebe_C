#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "gitfuncs.h"

char* itoa(int i, char b[]) //Tomado de http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
{
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    } while(i);
    return b;
}

void initCmd(char *a, DIR* b){
    if((strcmp(a,"init")) == 0){    
        if (b)
            printf("\nEl repositorio ya está inicializado.\n\n");            
        else if (ENOENT == errno){
        
            // Si el repositorio no existe, se crean los directorios.
            printf("\nCreando un nuevo repositorio de git...\n");
            mkdir(".git", ACCESSPERMS);
            mkdir(".git/commits", ACCESSPERMS); //Aquí se van almacenando los commits
            printf("\nRepositorio creado exitosamente.\n\n");
        }    
    }
}

void addCmd(char *a, char *b, int c){
    if((strcmp(a,"add")) == 0){     //Si se ingresó add.  
        if(c < 3){
            printf("\nFaltan argumentos.\n\n");
        }else if((strcmp(b,".")) == 0){ //Si el argumento es "."
            DIR *dir1;
            FILE *index=NULL;
            struct dirent *ent;
        //Se intenta abrir el directorio actual y el archivo de index.
            if ((dir1 = opendir ("./")) != NULL) {                
                if((index=fopen("./.git/index","a+")) == NULL){
                    printf("\nError al abrir archivo interno.");
                }else{
                    while ((ent = readdir (dir1)) != NULL) {
                        if((strcmp(ent->d_name,"git") != 0) && (strcmp(ent->d_name,".git") != 0) && (strcmp(ent->d_name,".") != 0) && (strcmp(ent->d_name,"..") != 0)){
                    //Se verificó qué archivos sí son archivos y se copiarán al index.
                            int cont = 0;
                            char temp[256];
                            rewind(index);
                        //Se checa si algún archivo ya está contenido en el index.
                            while(feof(index) == 0){
                                fscanf(index,"%s",temp);
                                if((strcmp(temp,ent->d_name)) == 0){
                                    cont=1;
                                    printf("El archivo '%s' ya había sido agregado.\n", ent->d_name);
                                }
                            }
                            rewind(index);
                        //Se agrega archivo al index si no existía en él.
                            if(cont==0)
                            {
                                fprintf(index,"%s\n", ent->d_name);
                                printf("El archivo '%s' se agregó con éxito.\n", ent->d_name);
                            }
                        }
                    }
                }
            closedir(dir1); 
            }else {
              // si no se puede abrir un directorio
                perror ("");
            }
        }
    //Si no se quieren agregar todos los archivos...
        else if ((strcmp(b, ".")) != 0){
            //Se verifica que el archivo a agregar exista
            if((fopen(b,"r") == NULL)){     
                printf("\nEl archivo no existe.\n\n");
            }else{
                FILE *index=NULL;           
            //Si el archivo existe y se puede abrir
                if((index=fopen("./.git/index","a+")) == NULL){
                    printf("\nError al abrir archivo interno.\n\n");
                }else{
            //Se veriica si el archivo ya estaba en index o no.
                    char temp[256];
                    while(feof(index) == 0){
                        fscanf(index,"%s",temp);
                        if((strcmp(temp,b)) == 0){
                            printf("\nEl archivo ya ha sido agregado antes.\n\n");
                            exit(0);
                        }
                    }
                //Si no estaba en index, se agrega.
                    fprintf(index,"%s\n", b);
                    printf("\nEl archivo '%s' se agregó con éxito.\n\n", b);
                }
            }     
        }
    }
}

void commitCmd(char *a, char *b, char **comentario){
    if((strcmp(a,"commit")) == 0)   //Si se ingresó commit
    {
        FILE *header = NULL;
        FILE *index = NULL;
        DIR *commits_dir;
        char numero[5];
        char file_name[256];
        char dir_name[256];
        char file_dir[512];
        char cp_cmd[1024] = {'\0'};
        char c;
        struct dirent *ent;
        int i = 3;
        int dir_name_int = 1;
        int next_dir = 0;

        if ((index = fopen("./.git/index","r")) == NULL || fgetc(index) == -1)
            printf("\nUsa el comando 'add' para seleccionar sobre qué archivos harás commit.\n\n");
        else
        {
            if(b == NULL)               //Si no se pusieron argumentos
                printf("\nIngresa la bandera -m seguida de tus comentarios.\n\n");
            else if(strcmp(b,"-m") == 0)//Si se puso -m
                if (comentario[i] == NULL) //pero no se pusieron comentarios
                    printf("\nFaltan los comentarios.\n\n");
                else //Hasta este punto se valida que todo está listo para hacer commits.
                {
                        if((header = fopen("./.git/header","a")) == NULL || (commits_dir = opendir("./.git/commits")) == NULL)
                            printf("\nError al abrir archivo interno.\n\n");
                        else
                        {
                        //Se crea una carpeta específica para este commit.
                            ent = readdir(commits_dir);
                            ent = readdir(commits_dir);
                            ent = readdir(commits_dir);
                            if (ent == NULL)
                            {
                                mkdir(".git/commits/1", ACCESSPERMS);
                                strcpy(dir_name, ".git/commits/1");
                            }
                            else
                            {
                                while (readdir(commits_dir) != NULL)
                                    dir_name_int ++;
                                dir_name_int ++;
                                strcpy(dir_name, ".git/commits/");
                                strcat(dir_name, itoa((dir_name_int), numero));
                                mkdir(dir_name, ACCESSPERMS);
                            }

                        //Se copian los archivos que están en index a la carpeta del nuevo commit.
                            rewind(index);
                            do
                            {
                                fscanf(index, " %[^\n]", file_name);
                                strcpy(cp_cmd, "cp ");
                                strcat(cp_cmd, file_name);
                                strcat(cp_cmd, " ./");
                                strcat(cp_cmd, dir_name);
                                system(cp_cmd);
                            } while (feof(index) == 0);
                                                    
                        //Se agregan los comentarios del commit al header. 
                            fprintf(header, "Commit #%i:\n\t", dir_name_int);       
                            while (comentario[i] != NULL)
                            {
                                fprintf(header, "%s ", comentario[i]);
                                i++;
                            }

                        //Se escribe en el header qué archivos se han modificado.
                            rewind(index);
                            fprintf(header, "\nArchivos modificados: ");
                            while (feof(index) == 0)
                            {
                                fscanf(index, " %[^\n]", file_name);
                                fprintf(header, "%s, ", file_name);
                            }
                            fprintf(header, "\n\n");

                            fclose(index);
                            index = fopen("./.git/index","w");
                        }
                        closedir(commits_dir); 
                        fclose(index);
                        fclose(header);
                        printf("\nCommit exitoso.\n\n");
                }
        }    
    }
}

void statusCmd(char *a)
{
    if((strcmp(a,"status")) == 0) //Si se ingresó status
    {   
        FILE *index = NULL;
        FILE *archivo_actual = NULL;
        FILE *archivo_commit = NULL;
        DIR *commits_dir;
        int dir_name_int = 0;
        int diferentes = 0;
        char dir_name[512] = {'\0'};
        char file_name[256] = {'\0'};
        char file_dir_orig[256] = {'\0'};
        char file_dir_commit[256] = {'\0'};
        char numero[5] = {'\0'};
        char c1;
        char c2;

        if ((commits_dir = opendir("./.git/commits")) == NULL || (index = fopen("./.git/index","r")) == NULL || fgetc(index) == -1)
            printf("\nUtiliza el comando 'add' primero.\n\n");
        else
        {
            //Se busca la carpeta donde está el último commit.
                rewind(index);
                while (readdir(commits_dir) != NULL)
                    dir_name_int ++;
                dir_name_int -= 2;
                strcpy(dir_name, ".git/commits/");
                strcat(dir_name, itoa((dir_name_int), numero));
                strcat(dir_name, "/");
                printf("\n");
            //Se leen los archivos en index y se trata de abrir cada uno tanto desde el repositorio como desde el último commit.
                while (feof(index) == 0)
                {
                    fscanf(index, " %[^\n]", file_name);
                    strcpy(file_dir_commit, dir_name);
                    strcat(file_dir_commit, file_name);
                    if ((archivo_actual = fopen(file_name,"rb")) == NULL || (archivo_commit = fopen(file_dir_commit,"rb")) == NULL)
                        printf("El archivo '%s' no estuvo en su último commit.\n", file_name);
                    else
                    {    
                //Se compara el mismo archivo desde las dos rutas. Si encuentra un sólo byte diferente entre ellos marca que hubo una modifcación.     
                        c1 = fgetc(archivo_actual);
                        c2 = fgetc(archivo_commit);
                        while (c1 != -1 || c2 != -1)
                        {
                            c1 = fgetc(archivo_actual);
                            c2 = fgetc(archivo_commit);

                            if (c1 != c2)
                            {
                                printf("El archivo '%s' ha sufrido modificaciones desde el último commit.\n", file_name);
                                diferentes = 1;
                                break;
                            }
                        }
                        if (diferentes == 0)
                            printf("El archivo '%s' no ha sufrido modificaciones desde el último commit.\n", file_name);
                    }
                }
            printf("\n");
        }
    }
}

void logCmd(char *a)
{
    if ((strcmp(a,"log")) == 0) //Si se ingresó log
    {   
        //Se imprime el archivo headers que contiene el log.
        FILE *log = NULL;
        char c;

        if ((log = fopen("./.git/header", "r")) == NULL)
            printf("\nNo se pudo abrir el archivo que contiene el log.\n\n");
        else
        {
            c = fgetc(log);
            while (c != -1)
            {
                printf("%c", c);
                c = fgetc(log);
            }
        }
    }
}

void otroCmd (char *a)
{   //Si la bandera no es ninguno de los comandos existentes, se listan las opciones posibles.
    if ((strcmp(a,"init")) != 0 && (strcmp(a,"add")) != 0 && (strcmp(a,"commit")) != 0 && (strcmp(a,"log")) != 0 && (strcmp(a,"status")) != 0)
    {
        printf("\nOpción inválida. Puedes utilizar:\n");
        printf("\tBandera 'init' para crear repositorio\n");
        printf("\tBandera 'add' para alistar archivos para hacer un commit.\n");
        printf("\tBandera 'commit' para hacer un commit.\n");
        printf("\tBandera 'log' para ver el registro de commits.\n");
        printf("\tBandera 'status' para verificar el estado actual de su repositorio.\n");
    }
}

void noArgs ()
{
    //Si no se pusieron argumentos, se listan las opciones posibles.
    printf("\nNo se ingresaron argumentos. Puedes utilizar:\n");
    printf("\tBandera 'init' para crear repositorio\n");
    printf("\tBandera 'add' para alistar archivos para hacer un commit.\n");
    printf("\tBandera 'commit' para hacer un commit.\n");
    printf("\tBandera 'log' para ver el registro de commits.\n");
    printf("\tBandera 'status' para verificar el estado actual de su repositorio.\n");
    exit(-1);
}
