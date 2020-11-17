#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	int pid;
	int n = atoi(argv[1]);

	if (argc != 2){
		printf("Número de parametros inválido\n");
		return 0;
	}

	if(n > 10 || n < 1){
		printf("Número de processos inválido\n");
		return 0;
	}

	printf("Sou o processo pai com pid = %d\n", getpid());	

	for(int i = 0; i < n; i++){

		if ((pid = fork()) < 0){
			printf("Erro no Fork\n");
			return 0;
		}

		if (pid == 0){
			sleep(30);
			break;
		}
		else{
			printf("Sou o processo filho com pid = %d\n", pid);	
		}	
	}
	return 0;
}