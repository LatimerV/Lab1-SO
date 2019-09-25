#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include <math.h>
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
	  printf("%f ",prom);
    }
	printf("\n");
  }
  return mf;
}

listF *leerPNG(char *nombre, listF *lf, int width, int height, png_byte color_type,
			  png_byte bit_depth, png_bytep *row_pointers) {
  FILE *archivo = fopen(nombre, "rb");
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png){ 
	abort();
  }
  png_infop info = png_create_info_struct(png);
  if(!info){ 
	abort();
  }
  if(setjmp(png_jmpbuf(png))){ 
	abort();
  }
  png_init_io(png, archivo);
  png_read_info(png, info);
  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);
  if(bit_depth == 16){
    png_set_strip_16(png);
  }
  if(color_type == PNG_COLOR_TYPE_PALETTE){
    png_set_palette_to_rgb(png);
  }
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){
    png_set_expand_gray_1_2_4_to_8(png);
  }
  if(png_get_valid(png, info, PNG_INFO_tRNS)){
    png_set_tRNS_to_alpha(png);
  }
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE){
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
  }
  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
    png_set_gray_to_rgb(png);
	 }
  png_read_update_info(png, info);
  if (row_pointers){
	abort(); 
  }
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
  /*read_png_file(argv[1]);*/
  /*process_png_file();*/
  /*write_png_file(argv[2]);*/

  return 0;
}

