#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int i;
	int pid, n;
	
	// Verifica o parâmetro fornecido
	if (argc < 2){
		printf("Forneça como parâmetro o número 'n' de processos que devem ser criados (1 <= n <= 10).\n");
		return 0;
	}

	n = atoi(argv[1]);
	if ((n < 1) || (n > 10)){
		printf("Número de processos inválido.\n");
		return 0;
	}	

	printf("Sou o processo pai com pid: %d\n", getpid());
	
	// Cria os processos filhos
	for (i = 0; i < n; i++){
		if ((pid = fork()) < 0){
			printf("Erro no fork.\n");
			exit(1);
		}

		if (pid == 0){
			sleep(20);
			printf ("Sou o processo filho com pid: %d\n", getpid());
			exit(0);
		}	
	}
	sleep(20);
	return 0;
}
