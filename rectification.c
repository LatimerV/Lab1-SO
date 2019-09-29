#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "matrixf.h"
#include "listf.h"


matrixF *rectification(matrixF *mf){
	for (int y = 0; y < countFil(mf); y++){
		for (int x = 0; x < countColumn(mf); x++){
			if (getDateMF(mf,y,x) < 0.0000){
				mf = setDateMF(mf, y, x, 0.0000);
			}
			if (getDateMF(mf,y,x) > 255.0000){
				mf = setDateMF(mf, y, x, 255.0000);
			}
		}
	}
	return mf;
}

int main(int argc, char *argv[]){
  printf("\n rectification\n");
	/* matrixf rectificacion;
	aqui iria la matriz para guardar la rectificacion*/	

  matrixF *entrada;
  matrixF *salida;

  char imagenArchivo[40]; /*Nombre del archivo imagen_1.png*/
  int umbralClasificacion[1]; /*numero del umbral*/

  pid_t pid;
  int status;


  int pUmbral[2]; /*para pasar el umbral para clasificacion*/
  int pNombre[2]; /*Para pasar nombre imagen_1.png*/
  //int pFiltroConvolucion[2]; /*para pasar filtro.txt*/
  int pImagen[2]; /*para pasar la imagen de rectificacion*/
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
    /*falta aqui read de la imagen desde convolucion*/
    read(5, entrada,sizeof(matrixF) );
    printf("rectification padre, umbral: %d y nombre: %s\n", umbralClasificacion[0], imagenArchivo);
    salida=rectification(entrada);
    
    
    /*Para pasar la imagen resultante de rectification*/
    close(pImagen[0]);
    write(pImagen[1],&salida,sizeof(matrixF));

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

     /*Para que el hijo (pooling) lea desde 5, la iamgen de rectification*/
    close(pImagen[1]);
    dup2(pImagen[0],5);



    char *argvHijo[] = {"pooling",NULL};
    execv(argvHijo[0],argvHijo);
  }
    return 0;


	

}
