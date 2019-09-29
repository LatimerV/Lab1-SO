#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	printf("dentro del codigo de hijo\n");
	for (int y = 0; y < 10; y++){
		int pipefd1[2];
		int pipefd2[2];
		pipe(pipefd1);
		pipe(pipefd2);
		int sum = 0;
		read(3, &sum, sizeof(sum));
		sum = sum * 2;
		write(4, &sum, sizeof(sum));
	}
	return 0;
}