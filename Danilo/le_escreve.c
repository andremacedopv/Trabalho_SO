#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

struct sembuf operacao[2];
int write_sem, read_sem;

// Operações do semáforo

int p_sem(int idsem){
  operacao[0].sem_num = 0;
  operacao[0].sem_op = 0;
  operacao[0].sem_flg = 0;
  operacao[1].sem_num = 0;
  operacao[1].sem_op = 1;
  operacao[1].sem_flg = 0;
  if (semop(idsem, operacao, 2) < 0){
    printf("erro no p=%d\n", errno);
  }  
   return 0;
}

int v_sem(int idsem){
  operacao[0].sem_num = 0;
  operacao[0].sem_op = -1;
  operacao[0].sem_flg = 0;
  if (semop(idsem, operacao, 1) < 0){
    printf("erro no v=%d\n", errno);
  }
  return 0;
}

int main(){
  int pid, idshm, estado;
  // struct shmid_ds buf;
  int *pshm;

  // Cria memória compartilhada
  if ((idshm = shmget(0x555, sizeof(int), IPC_CREAT|0x1ff)) < 0){
    printf("erro na criacao da fila\n");
    exit(1);
  }

  // Cria semáforo de escrita
  if ((write_sem = semget(0x1111, 1, IPC_CREAT|0x1ff)) < 0){
    printf("erro na criacao do semáforo\n");
    exit(1);
  } 
  // Cria semáforo de leitura
  if ((read_sem = semget(0x1112, 1, IPC_CREAT|0x1ff)) < 0){
    printf("erro na criacao do semáforo\n");
    exit(1);
  } 
  
  p_sem(read_sem);

  // Cria processo filho
  if ((pid = fork()) < 0){
    printf("erro no fork\n");
    exit(1);
  }
  // Processo filho (P1)
  if (pid == 0){

    pshm = (int *) shmat(idshm, (char *)0, 0);
    if (pshm == (int *)-1){
      printf("erro no attach\n");
      exit(1);
    }
    for (int i = 1; i <= 20; i++){
      p_sem(write_sem);
      *pshm = i;
      printf("processo %d escreveu %d\n", getpid(), i);
      v_sem(read_sem);
    }

    exit(0);
  }

  // Processo pai (P2)
  pshm = (int *) shmat(idshm, (char *)0, 0);
  if (pshm == (int *)-1){
    printf("erro no attach\n");
    exit(1);
  }
  for (int i = 1; i <= 20; i++){
    p_sem(read_sem);
    printf("processo %d leu %d\n", getpid(), *pshm);
    v_sem(write_sem);
  }

  wait(&estado);

  // Remove o segmento de memória compartilhado
  shmctl(idshm, IPC_RMID, NULL);
  // Remove semáforo de leitura
  semctl(read_sem, 0, IPC_RMID, 0);
  // Remove semáforo de escrita
  semctl(write_sem, 0, IPC_RMID, 0);

  exit(0);
  
  return 0;
}