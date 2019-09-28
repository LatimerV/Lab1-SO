#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "matrixf.h"
#include "listf.h"

matrixF *pooling(matrixF *mf){
	int heigth = 0, width = 0;
	if (countFil(mf)%2 == 0){
		heigth = countFil(mf)/2;
	}
	if (countFil(mf)%2 == 1){
		heigth = (countFil(mf)/2) + 1;
	}
	if (countColumn(mf)%2 == 0){
		width = countColumn(mf)/2;
	}
	if (countColumn(mf)%2 == 1){
		width = (countColumn(mf)/2) + 1;
	}
	matrixF *newmf = createMF(heigth, width);
	int fil = 0, col = 0, fil2 = 0, col2 = 0;
	while (fil < countFil(mf)){
		col2 = 0;
		while (col < countColumn(mf)){
			float max = 0.0000;
			for (int y = 0; y < 2; y++){
				for (int x = 0; x < 2; x++){
					if (max > getDateMF(mf, y, x)){
						max = getDateMF(mf, y, x);
					}
				}
			}
			newmf = setDateMF(newmf, fil2, col2, max);
			col = col + 2;
			col2 = col2 + 1;
		}
		fil = fil + 2;
		fil2 = fil2 + 1;
	}
	return newmf;
}


int main(int argc, char *argv[]){

	/* matrixf poolong;
	aqui iria la matriz para guardar el pooling*/	

  char imagenArchivo[40]; /*Nombre del archivo imagen_1.png*/
  int umbralClasificacion[1]; /*numero del umbral*/

  pid_t pid;
  int status;


  int pUmbral[2]; /*para pasar el umbral para clasificacion*/
  int pNombre[2]; /*Para pasar nombre imagen_1.png*/
  //int pFiltroConvolucion[2]; /*para pasar filtro.txt*/
  int pImagen[2]; /*para pasar la imagen de pooling*/
  /*Se crean los pipes*/
  //pipe(pFiltroConvolucion);
  pipe(pUmbral);
  pipe(pNombre);
  pipe(pImagen);
  
  /*Se crea el proceso hijo.*/
  pid = fork();
  
  /*Es el padre*/
  if(pid>0){

    read(3,imagenArchivo,sizeof(imagenArchivo));
    read(4,umbralClasificacion,sizeof(umbralClasificacion));
    /*falta aqui read de la imagen desde rectification*/
    //read(5, nombreFiltroConvolucion,sizeof(nombreFiltroConvolucion) );

    /*pooling(matrixF *mf)*/
    
    
    /*Para pasar la imagen resultante de pooling
    close(pImagen[0]);
    write(pImagen[1],lf,sizeof(listF));*/

    close(pNombre[0]);
    write(pNombre[1],imagenArchivo,(strlen(imagenArchivo)+1));

    close(pUmbral[0]);
    write(pUmbral[1],umbralClasificacion,sizeof(umbralClasificacion));

    waitpid(pid,&status,0);

  }else{ /*Es el hijo*/

    
    close(pNombre[1]);
    dup2(pNombre[0],3);

    close(pUmbral[1]);
    dup2(pUmbral[0],4);

     /*Para que el hijo (classification) lea desde 5, la iamgen de pooling
    close(pImagen[1]);
    dup2(pImagen[0],5);*/



    char *argvHijo[] = {"classification",NULL};
    execv(argvHijo[0],argvHijo);
  }
    return 0;


	

}
