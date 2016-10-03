#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "gitfuncs.h"

int main (int argc, char *argv[]){
    // Para guardar un buffer
    if (argc < 2)
        noArgs();
    char buff[128];
    memset(buff,0,sizeof(buff));

    // copia el comando actual a buff
    strcpy(buff,argv[1]);
    
    DIR* dir = opendir(".git");
    
    if (dir){
        initCmd(buff,dir);
        addCmd(buff,argv[2],argc);
        commitCmd(buff,argv[2],(char**)argv);
        statusCmd(buff);
        logCmd(buff);
        otroCmd(buff);
        closedir(dir);
    }else if (ENOENT == errno){
        printf("\nAún no has creado un repositorio. Utiliza el comando 'init' para hacerlo.\n");
        initCmd(buff,dir);
    }
    printf("\n");
    
    return 0;
}