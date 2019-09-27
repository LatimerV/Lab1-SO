#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include <math.h>
#include "matrix.h"
#include "listf.h"

void escribirPNG(char *filename, matrixF *mf) {
  int y;
  FILE *filepng = fopen(nombre, "rb");
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);
  png_init_io(png, filepng);
  png_set_IHDR(
    png,
    info,
    countColumn(mf), countFil(mf),
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * countFil(mf));
  for(int y = 0; y < countFil(mf); y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }
  png_read_image(png, row_pointers);
  for(int y = 0; y < countFil(mf); y++) {
    for(int x = 0; x < countColumn(mf); x++) {
      row_pointers[y][x * 4][0] = getDateMF(mf, y, x);
	  row_pointers[y][x * 4][1] = getDateMF(mf, y, x);
	  row_pointers[y][x * 4][2] = getDateMF(mf, y, x);
    }
  }
  png_write_info(png, info);  
  png_write_image(png, row_pointers);
  png_write_end(png, NULL); 
  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);
  fclose(filepng);
  png_destroy_write_struct(&png, &info);
}

int classification(matrixF *mf, int umbral, char *namefile){
	int maxBlack = 0;
	for (int y = 0; y < countFil(mf); y++){
		for (int x = 0; x < countColumn(mf); x++){
			if (getDateMF(mf, y, x) == 0.0000){
				maxBlack = maxBlack + 1;
			}
		}
	}
	float porcentBlack = (maxBlack * 100.0000)/(countFil(mf) * countColumn(mf));
	if (porcentBlack >= umbral){
		printf("|   %s   |         yes        |\n",namefile);
	}
	if {
		printf("|   %s   |         no         |\n",namefile);
	}
	escribirPNG(mf, namefile);
	return 0;
}