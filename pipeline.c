#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include <math.h>
#include <unistd.h>
#include "matrixf.h"
#include "listf.h"





void recibirParametros(int argc, char *argv[], int *n, int *flag){
    char *cflag = (char*)malloc(100*sizeof(char));
    char *mflag = (char*)malloc(100*sizeof(char));
    char *nflag = (char*)malloc(100*sizeof(char));
    int numeroImagenes=0;
    int umbralClasificacion =0;

    int pUmbral[2];
    int pNombre[2];
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
  umbralClasificacion = atoi(nflag);




  while(numeroImagenes>0){
    char cantidadImg[10];
    sprintf(cantidadImg,"%d",numeroImagenes); 
    char imagenArchivo[] = "imagen_"; /*Archivo de entrada imagenes*/
    char extension[] = ".png"; /*Extension de imagen*/
    strcat(imagenArchivo,cantidadImg); /*imagen_1*/
    strcat(imagenArchivo,extension); /*imagen_1.png*/
    
    /*Se crean los pipes*/
    pipe(pNombre);
    pipe(pUmbral);
    
    /*Se crea el proceso hijo*/
    pid = fork();
    
    /*Mayor que 0 es el PADRE*/
    if(pid>0){
      close(pNombre[0]); /*Se cierra la lectura*/
      write(pNombre[1],imagenArchivo,(strlen(imagenArchivo)+1));
      close(pUmbral[0]); /*Se cierra la lectura*/
      write(pUmbral[1],umbralClasificacion,sizeof(umbralClasificacion));
      waitpid(pid,status,0);

    }else{/*Es hijo*/
      close(pNombre[1]); /*Se cierra la escritura*/
      dup2(pNombre[0],3);
      close(pUmbral[1]); /*Se cierra la escritura*/
      dup2(pUmbral[0],4);

      char *argvHijo[] = {"lectura",NULL}; /*Nombre del archivo al cual pasara el hijo*/
      execv(argvHijo[0],argvHijo); /*Reemplaza el codigo del proceso, por el cual apunta argvHijo*/
    }
      numeroImagenes--;
  }
      
    
  
}/*Fin Recibir Parametros*/



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

listF *leerPNG(char *nombre, listF *lf, int width, int height, png_byte color_type,
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
  return lf;
}

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
}

