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
	printf("codigo de ejemplo\n");
	int sum = 1, status;
	int pipefd1[2];
	int pipefd2[2];
	pipe(pipefd1);
	pipe(pipefd2);
	pid_t pid;
	pid = fork();
	if (pid > 0){
		printf("este es el padre %d\n", pid);
		close(pipefd1[0]);
		for (int x = 0; x < 10; x++){
			write(pipefd1[1],&sum, sizeof(sum));
			close(pipefd2[1]);
			read(pipefd2[0],&sum, sizeof(sum));
			sum = sum + 1;
			printf("resultado: %d\n", sum);
		}
	}
	else if (pid == 0){
		printf("este es el hijo %d\n", pid);
		close(pipefd1[1]);
		dup2(pipefd1[0],3);
		close(pipefd2[0]);
		dup2(pipefd2[1],4);
		char *argvHijo[] = {"hijofork",NULL};
		execv(argvHijo[0],argvHijo);
		perror("exec ls failed");
		exit(EXIT_FAILURE);
	}
}
