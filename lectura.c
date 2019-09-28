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



matrixF *grayScale(png_bytep *row_pointers, int height, int width) {
  matrixF *mf = createMF(height, width);
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);
	  float prom = px[0]*0.299+px[1]*0.587+px[2]*0.114;
	  mf = setDateMF(mf, y, x, prom);
    }
  }
  for (int a = 0; a < height; a++){
	for (int b = 0; b < width; b++){
		printf("%f ",getDateMF(mf, a, b));
	}
	printf("\n");
  }
  return mf;
}

void leerPNG(char *nombre, listF *lf, int width, int height, png_byte color_type,
			  png_byte bit_depth, png_bytep *row_pointers) {
  FILE *archivo = fopen(nombre, "rb");
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);
  png_init_io(png, archivo);
  png_read_info(png, info);
  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);
  png_read_update_info(png, info);
  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }
  png_read_image(png, row_pointers);
  fclose(archivo);
  png_destroy_read_struct(&png, &info, NULL);
  matrixF *mf = grayScale(row_pointers, height, width);
  lf = insertLF(lf, mf, longLF(lf));
  //return lf;
}

/*
int main(int argc, char *argv[]) {
  int width, height;
  png_byte color_type;
  png_byte bit_depth;
  png_bytep *row_pointers = NULL;
  listF *lf = createLF(NULL);
  if(argc != 2){	 
	abort();
  }
  lf = leerPNG(argv[1], lf, width, height, color_type, bit_depth, row_pointers);
  return 0;
}*/




int main(int argc, char *argv[]){

  listF *lf = createLF(NULL); /*Imagen Png*/
  int width, height;
  png_byte color_type;
  png_byte bit_depth;
  png_bytep *row_pointers = NULL;

  char imagenArchivo[40]; /*Nombre del archivo imagen_1.png*/
  char nombreFiltroConvolucion[40]; /*filtro.txt*/
  int umbralClasificacion[1]; /*numero del umbral*/

  pid_t pid;
  int status;


  int pUmbral[2]; /*para pasar el umbral para clasificacion*/
  int pNombre[2]; /*Para pasar nombre imagen_1.png*/
  int pFiltroConvolucion[2]; /*para pasar filtro.txt*/
  int pImagen[2]; /*para pasar la imagen de lectura*/
  /*Se crean los pipes*/
  pipe(pFiltroConvolucion);
  pipe(pNombre);
  pipe(pUmbral);
  pipe(pImagen);
  
  /*Se crea el proceso hijo.*/
  pid = fork();
  
  /*Es el padre*/
  if(pid>0){
    read(3,imagenArchivo,sizeof(imagenArchivo));
    read(4,umbralClasificacion,sizeof(umbralClasificacion));
    read(5, nombreFiltroConvolucion,sizeof(nombreFiltroConvolucion) );
    leerPNG(imagenArchivo, lf, width, height, color_type, bit_depth, row_pointers);
    
    close(pImagen[0]);
    write(pImagen[1],lf,sizeof(listF));

    close(pNombre[0]);
    write(pNombre[1],imagenArchivo,(strlen(imagenArchivo)+1));

    close(pUmbral[0]);
    write(pUmbral[1],umbralClasificacion,sizeof(umbralClasificacion));

    close(pFiltroConvolucion[0]);
    write(pFiltroConvolucion[1],nombreFiltroConvolucion,(strlen(nombreFiltroConvolucion)+1));

    waitpid(pid,&status,0);

  }else{ /*Es el hijo*/

    close(pImagen[1]);
    dup2(pImagen[0],3);
    
    close(pNombre[1]);
    dup2(pNombre[0],4);

    close(pUmbral[1]);
    dup2(pUmbral[0],5);

    close(pFiltroConvolucion[1]);
    dup2(pFiltroConvolucion[0],6);



    char *argvHijo[] = {"bidirectionalConvolution",NULL};
    execv(argvHijo[0],argvHijo);
  }
    return 0;

}

