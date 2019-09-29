padre: main.c lectura.c bidireccionalConvolution.c -lpng
	gcc -o main main.c -I.
	gcc -o lectura lectura.c -I.

