#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>
//#include "funct.h"
//#include "structs.h"

int main(int argc, char *argv[]){
    //printf("Hello World");
    
    int n = 0, flag = 0;
    recibirParametros(argc, argv, &n, &flag);
    //crearProcesos(4);
    return 0;
}