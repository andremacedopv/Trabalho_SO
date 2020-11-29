// Trabalho 2 SO - Sincronização Two-Way

// Alunos: André Macedo Prado Valle - 17/0005950
//         Felipe Lima Vaz - 17/0057852
//         Danilo Inácio dos Santos Silva - 17/0031861
// Versões do SO testadas: Ubuntu 20.04
// Versões do compilador testadas: gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>

const int N_SIZE = 20;

// Controladores do semáforo de leitura e escrita
struct sembuf operation[2];
int read_sem, wrt_sem;

void p_sem(int sem_id) {
  operation[0].sem_num = 0;
  operation[0].sem_op = 0;
  operation[0].sem_flg = 0;
  operation[1].sem_num = 0;
  operation[1].sem_op = 1;
  operation[1].sem_flg = 0;
  if(semop(sem_id, operation, 2) < 0)
    printf("Erro no P: %d\n", errno);
}

void v_sem(int sem_id) {
  operation[0].sem_num = 0;
  operation[0].sem_op = -1;
  operation[0].sem_flg = 0;
  if(semop(sem_id, operation, 1) < 0)
    printf("Erro no V: %d\n", errno);
}

// Gerador de números aleatórios
/* Foi optado por, ao invés de enviar os números em ordem de 1 a 20,
   envia-los em ordem aleatória, para melhor demonstrar a entrega
   das mensagens.
*/
void get_random_array(int n[]) {
  // Get rand with time
  srand((unsigned) time(NULL));
  int base_rdm = rand();

  // Populate array
  for(int i = 0; i < N_SIZE; i++)
    n[i] = i + 1;

  // Shuffle array
  for(int i = 0; i < N_SIZE; i++) {
    int j = i + base_rdm / (RAND_MAX / (N_SIZE - i) + 1);
    int t = n[j];
    n[j] = n[i];
    n[i] = t;
  }
}

int main() {
  int pid, state, idshm;
  int *pshm;

  // Obter memória compartilhada
  if((idshm = shmget(0x5950, sizeof(int), IPC_CREAT|0x1ff)) < 0){
    printf("Erro na obtenção da memória compartilhada.\n");
    exit(1);
  }

  // Obter semáforo de controle de leitura
  if((read_sem = semget(0x1103, 1, IPC_CREAT|0x1ff)) < 0) {
    printf("Erro na criação do semáforo de leitura.\n");
    exit(1);
  }

  // Obter semáforo de controle de escrita
  if((wrt_sem = semget(0x1999, 1, IPC_CREAT|0x1ff)) < 0) {
    printf("Erro na criação do semáforo de escrita.\n");
    exit(1);
  }

  // Impedir leitura precipitada pelo P2
  p_sem(read_sem);

  // Gerar P1
  if((pid = fork()) < 0) {
    printf("Erro no fork.\n");
    exit(1);
  }

  if(pid == 0) {
    // Código do P1

    // Attach na memória compartilhada
    pshm = (int *) shmat(idshm, (char *)0, 0);
    if(pshm == (int *)-1) {
      printf("Erro no attach do P1.\n");
      exit(1);
    }

    // Escrita na memória compartilhada
    int numbers[N_SIZE];
    get_random_array(numbers);
    for(int i = 0; i < N_SIZE; i++) {
      p_sem(wrt_sem);
      *pshm = numbers[i];
      printf("%i: Processo %d escreveu %d\n", i, getpid(), *pshm);
      v_sem(read_sem);
    }

    exit(0);
  }

  // Código do P2

  // Attach na memória compartilhada
  pshm = (int *) shmat(idshm, (char *)0, 0);
  if(pshm == (int *)-1) {
    printf("Erro no attach do P2.\n");
    exit(1);
  }

  // Leitura na memória compartilhadaa
  for(int i = 0; i < N_SIZE; i++) {
    p_sem(read_sem);
    printf("Processo %d leu %d.\n", getpid(), *pshm);
    v_sem(wrt_sem);
  }

  // Remover recursos alocados
  if(shmctl(idshm, IPC_RMID, NULL) < 0) {
    printf("Erro na remoção da memoria compartilhada.\n");
  }

  if(semctl(read_sem, 0, IPC_RMID, NULL) == -1) {
    printf("Erro na remoção do semáforo de leitura.\n");
  }
  
  if (semctl(wrt_sem, 0, IPC_RMID, NULL) == -1) {
    printf("Erro na remoção do semáforo de escrita.\n");
  }

  wait(&state);
  return 0;
}