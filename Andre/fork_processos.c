// Trabalho 1 SO - Criação de Processos

// Alunos: André Macedo Prado Valle - 17/0005950
//         Felipe Lima Vaz - 17/0057852
//         Danilo Inácio dos Santos Silva - 17/0031861
// Versões do SO testadas: Ubuntu 20.04
// Versões do compilador testadas: gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  // Verificar se parâmetro foi passado
  if(argc < 2) {
    printf("Passe o número de processos a ser criado pela linha de comando (1 <= n <= 10).\n");
    printf("Ex: ./fork_processos 7\n");
    return 0;
  }

  // Verificar valor de 'n' passado
  int n = atoi(argv[1]);

  if(n < 1 ||n > 10) {
    printf("O valor de n não deve ser maior que 10 ou menor que 1.\n");
  }

  // Imprimir informações sobre o pai
  printf("Sou o processo pai com pid = %d\n", getpid());

  // Executar os forks
  // int *pid = (int *) calloc(n, sizeof(int));
  int pid = 0;
  for(int i = 0; i < n; i++) {
    if((pid = fork()) < 0) {
      printf("Erro no fork.\n");
      exit(1);
    }

    if(pid == 0) {
      printf("Sou o processo filho com pid = %d\n", getpid());
      sleep(30);
      exit(7);
    }
    pid = 0;
  }

  // Esperar os processos filhos
  int state;
  for(int i = 0; i < n; i++) {
    wait(&state);
  }

  return 0;
}