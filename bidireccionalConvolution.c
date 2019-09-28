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

matrixF *bidirectionalConvolution(matrixF *mf, matrixF *filter){
	if ((countFil(filter) == countColumn(filter))&&(countFil(filter)%2 == 1)){
		int increase = 0, initial = countFil(filter);
		while (initial != 1){
			initial = initial - 2;
			increase = increase + 1;
		}
		for (int cont = 0; cont < increase; cont++){
			mf = amplifyMF(mf);
		}
		for (int fil = 0; fil < countFil(filter) - countFil(filter); fil++){
			for (int col = 0; col < countColumn(filter) - countColumn(filter); col++){
				float sum = 0.0000;
				for (int y = 0; y < countFil(filter); y++){
					for (int x = 0; x < countColumn(filter); x++){
						float result = getDateMF(mf, y, x)*getDateMF(filter, y + fil, x + col);
						sum = sum + result;
					}
				}
				mf = setDateMF(mf, fil + (countFil(filter)/2), col + (countColumn(filter)/2), sum);
			}
		}
		for (int cont2 = 0; cont2 < increase; cont2++){
			mf = decreaseMF(mf);
		}
		return mf;
	}
	else{
		return mf;
	}
}



int main(int argc, char *argv[]){

  /*aqui iria la matriz para guardar la convolucion*/	

  char imagenArchivo[40]; /*Nombre del archivo imagen_1.png*/
  char nombreFiltroConvolucion[40]; /*filtro.txt*/
  int umbralClasificacion[1]; /*numero del umbral*/

  pid_t pid;
  int status;


  int pUmbral[2]; /*para pasar el umbral para clasificacion*/
  int pNombre[2]; /*Para pasar nombre imagen_1.png*/
  //int pFiltroConvolucion[2]; /*para pasar filtro.txt*/
  int pImagen[2]; /*para pasar la imagen de lectura*/
  /*Se crean los pipes*/
  //pipe(pFiltroConvolucion);
  pipe(pNombre);
  pipe(pUmbral);
  pipe(pImagen);
  
  /*Se crea el proceso hijo.*/
  pid = fork();
  
  /*Es el padre*/
  if(pid>0){

    read(3,imagenArchivo,sizeof(imagenArchivo));
    read(4,umbralClasificacion,sizeof(umbralClasificacion));
    /*falta aqui read de la imagen desde lectura*/
    //read(5, nombreFiltroConvolucion,sizeof(nombreFiltroConvolucion) );
    /*bidirectionalConvolution(matrixF *mf, matrixF *filter)*/
    
    
    /*Para pasar la imagen resultante de convolucion
    close(pImagen[0]);
    write(pImagen[1],lf,sizeof(listF));*/

    close(pNombre[0]);
    write(pNombre[1],imagenArchivo,(strlen(imagenArchivo)+1));

    close(pUmbral[0]);
    write(pUmbral[1],umbralClasificacion,sizeof(umbralClasificacion));

    waitpid(pid,&status,0);

  }else{ /*Es el hijo*/

    /*Para que el hijo lea desde 3, la iamgen de convolucion
    close(pImagen[1]);
    dup2(pImagen[0],3);*/
    
    close(pNombre[1]);
    dup2(pNombre[0],4);

    close(pUmbral[1]);
    dup2(pUmbral[0],5);



    char *argvHijo[] = {"rectification",NULL};
    execv(argvHijo[0],argvHijo);
  }
    return 0;

}

