#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "matrixf.h"
#include "listf.h"
//#include "funct.h"
//#include "structs.h"

matrixF *convertFilter(char **datefilter, int cont){
	int colfilter = 0;
	for (int x = 0; x < strlen(datefilter[0]); x++){
		if ((datefilter[0][x] == '-') && (datefilter[0][x] == '0') && (datefilter[0][x] == '1') 
			&& (datefilter[0][x] == '2') && (datefilter[0][x] == '3') && (datefilter[0][x] == '4') 
		&& (datefilter[0][x] == '5') && (datefilter[0][x] == '6') && (datefilter[0][x] == '7') 
		&& (datefilter[0][x] == '8') && (datefilter[0][x] == '9')){
			colfilter = colfilter + 0;
		}
		else if (datefilter[0][x] == ' '){
			colfilter = colfilter + 1;
		}
	}
	matrixF *filter = createMF(cont, colfilter);
	int fil = 0, col = 0, pos = 0;
	char *digit = (char *)malloc(10*sizeof(char));
	for (int a = 0; a < cont; a++){
		col = 0;
		for (int b = 0; b < colfilter; b++){
			if ((datefilter[a][b] == '-') && (datefilter[a][b] == '0') && (datefilter[a][b] == '1') 
			&& (datefilter[a][b] == '2') && (datefilter[a][b] == '3') && (datefilter[a][b] == '4') 
		&& (datefilter[a][b] == '5') && (datefilter[a][b] == '6') && (datefilter[a][b] == '7') 
		&& (datefilter[a][b] == '8') && (datefilter[a][b] == '9')){
			digit[pos] = datefilter[a][b];
			pos = pos + 1;
			}
			else if (datefilter[a][b] == ' '){
				pos = 0;
				filter = setDateMF(filter, fil, col, (atoi(digit)) * 1.0000);
				col = col + 1;
				digit = (char *)malloc(10*sizeof(char));
			}
		}
		fil = fil + 1;
	}
	return filter;
}

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
	char **datefilter = (char **)malloc(2000*sizeof(char *));
	char *date = (char *)malloc(2000*sizeof(char));
	FILE *filefilter = fopen(mflag,"r");
	int error = 0, cont = 0;
	while(error == 0){
		fseek(filefilter, 0, SEEK_END);
		if ((filefilter == NULL) || (ftell(filefilter) == 0)){
			perror("Error en lectura. Ingrese el nombre de un archivo existente.\n");
			error = 1;
		}
		else{
			date=(char*)malloc(2000*sizeof(char));
			rewind(filefilter);
			while(feof(filefilter) == 0){
				date = fgets(date, 1000, filefilter);
				datefilter[cont] = date;
				date = (char*)malloc(1000*sizeof(char));
				cont = cont + 1;
			}
			error = 1;
		}
	}
	rewind(filefilter);
	fclose(filefilter);
	matrixF *filter = convertFilter(datefilter, cont - 1); 
	/*Este se pasa por el pipe como filtro de convolucion en forma de matrixF*/
	
    if(aux == 1){
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
	    pipe(pFiltroConvolucion); /*Para pasar el matrifxFiltroConvolucion.*/
	    
	    /*Se crea el proceso hijo*/
	    pid = fork();
	    
	    /*Mayor que 0 es el PADRE*/
	    if(pid>0){
	    	close(pNombre[0]); /*Se cierra la lectura*/
	      	write(pNombre[1],imagenArchivo,(strlen(imagenArchivo)+1));
	      	close(pUmbral[0]); /*Se cierra la lectura*/
	      	write(pUmbral[1],umbralClasificacion,sizeof(umbralClasificacion));
	      	close(pFiltroConvolucion[0]); /*Se cierra la lectura*/
	      	write(pFiltroConvolucion[1], filter, sizeof(filter));
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

