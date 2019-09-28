#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
//#include "funct.h"
//#include "structs.h"

int main(int argc, char *argv[]){

    char *cflag = (char*)malloc(100*sizeof(char));
    char *mflag = (char*)malloc(100*sizeof(char));
    char *nflag = (char*)malloc(100*sizeof(char));
    int numeroImagenes=0;
    int umbralClasificacion[1];

    int pUmbral[2];
    int pNombre[2];
    int pFiltroConvolucion[2];
    int status;
    pid_t pid;

    
    int caso, aux=0;
    while((caso=getopt(argc,argv, "c:m:n:b"))!= -1){
        switch(caso){
            case 'c':
        
                strcpy(cflag, optarg); /*Numero Cantidad imagenes*/
        
                break;    
            case 'm':
                strcpy(mflag, optarg); /*Archivo mascara filtro .txt*/
                break;  
            
            case 'n':
                strcpy(nflag, optarg); /*Numero Umbral clasificacion*/
                break;   
        
            case 'b': /*Se muestra o no por pantalla*/
                aux=1;
                break;
             default:
                abort();
            
            
        }   
    }
    if(aux==1){
    }


    numeroImagenes = atoi(cflag);
  	umbralClasificacion[0] = atoi(nflag);

  	printf("\n|     Imagen     |     Nearly Black     |\n");
  	while(numeroImagenes>0){ /*Se ejecuta while miestras sea numeroImagenes>0*/
	    char cantidadImg[10];
	    sprintf(cantidadImg,"%d",numeroImagenes); 
	    char *nombreFiltroConvolucion= mflag; /*Archivo para la etapa de convolucion*/
	    char imagenArchivo[] = "imagen_"; /*Archivo de entrada imagenes*/
	    char extension[] = ".png"; /*Extension de imagen*/
	    strcat(imagenArchivo,cantidadImg); /*imagen_1*/
	    strcat(imagenArchivo,extension); /*imagen_1.png*/
	    
	    /*Se crean los pipes*/
	    pipe(pNombre); /*Para pasar el nombreImagen.*/
	    pipe(pUmbral); /*Para pasar el umbral para clasificacion.*/
	    pipe(pFiltroConvolucion); /*Para pasar el nombreFiltroConvolucion.*/
	    
	    /*Se crea el proceso hijo*/
	    pid = fork();
	    
	    /*Mayor que 0 es el PADRE*/
	    if(pid>0){
	    	close(pNombre[0]); /*Se cierra la lectura*/
	      	write(pNombre[1],imagenArchivo,(strlen(imagenArchivo)+1));
	      	close(pUmbral[0]); /*Se cierra la lectura*/
	      	write(pUmbral[1],umbralClasificacion,sizeof(umbralClasificacion));
	      	close(pFiltroConvolucion[0]); /*Se cierra la lectura*/
	      	write(pFiltroConvolucion[1], nombreFiltroConvolucion, (strlen(nombreFiltroConvolucion)+1));
	      	waitpid(pid,&status,0);

	    }else{/*Es hijo*/
	      	close(pNombre[1]); /*Se cierra la escritura*/
	      	dup2(pNombre[0],3);
	      	close(pUmbral[1]); /*Se cierra la escritura*/
	      	dup2(pUmbral[0],4);
	      	close(pFiltroConvolucion[1]); /*Se cierra escritura*/
	      	dup2(pFiltroConvolucion[0], 5);
	      	char *argvHijo[] = {"lectura",NULL}; /*Nombre del archivo al cual pasara el hijo*/
	      	execv(argvHijo[0],argvHijo); /*Reemplaza el codigo del proceso, por el cual apunta argvHijo*/
	    }
	      numeroImagenes--;
  	}/*Fin while*/


}/*Fin Main*/

