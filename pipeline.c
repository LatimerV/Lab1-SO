#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include <math.h>
#include <unistd.h>
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

