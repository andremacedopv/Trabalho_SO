// Trabalho 3 SO - Temporizador

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
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <signal.h>

const int N_SIZE = 10;

typedef struct {
  long pid;
  char msg[60];
} TypeMsg;

// Rotina de tratamento do alarme
void alarm_treatment(){
  printf("Ocorreu Timeout - Não recebi mensagem em 2 segundos.\n");
}

// Gerador de números aleatórios
void get_random_array(int n[]) {
  // Get rand with time
  srand((unsigned) time(NULL));
  int base_rdm = rand();

  // Populate array with random values
  for(int i = 0; i < N_SIZE; i++)
    n[i] = (rand() % 4) + 1;

  // Shuffle array
  for(int i = 0; i < N_SIZE; i++) {
    int j = i + base_rdm / (RAND_MAX / (N_SIZE - i) + 1);
    int t = n[j];
    n[j] = n[i];
    n[i] = t;
  }
  printf("Tempos de sleep:\n");

  for(int i = 0; i < N_SIZE; i++)
    printf("%d ", n[i]);

  printf("\n----------------\n");
}

int main() {
  int pid, state, msg_id;
  TypeMsg msg_snd, msg_get;
  char msg_temp[30];

  // Obter array de tempos
  int times[N_SIZE];
  get_random_array(times);

  // Definir rotina de tratamento para o alarme
  signal(SIGALRM, alarm_treatment);

  // Obter fila de mensagens
  if((msg_id = msgget(0x5950, IPC_CREAT|0x1ff)) < 0){
    printf("Erro na obtenção da fila.\n");
    exit(1);
  }

  // Gerar P2
  if((pid = fork()) < 0) {
    printf("Erro no fork.\n");
    exit(1);
  }

  if(pid == 0) {
    // Código do P2
    for(int i = 0; i < N_SIZE; i++) {
      // Definir mensagem a ser transmitida
      msg_snd.pid = getpid();
      sprintf(msg_temp, "pid=%ld numero=%d", msg_snd.pid, i);
      strcpy(msg_snd.msg, msg_temp);
      
      sleep(times[i]);

      // Envio da mensagm
      msgsnd(msg_id, &msg_snd, sizeof(msg_snd), 0);
      printf("Mensagem %s enviada.\n", msg_snd.msg);
    }

    exit(0);
  }

  // Código do P1
  for(int i = 0; i < N_SIZE; i++) {
    // Acionar alarme para 2 segundos
    alarm(2);

    // Tentar oter mensagem
    if(msgrcv(msg_id, &msg_get, sizeof(msg_get), 0, 0) > 0){
      alarm(0);
      printf("Recebi mensagem %s.\n", msg_get.msg);
    }
    else {
      // Imprimir erro não relacionado ao alarme
      if(errno != EINTR) {
        printf("Falha na recepção da mensagem.\n");
      }
    }
  }

  // Remover recursos alocados
  if(msgctl(msg_id, IPC_RMID, NULL) < 0) {
    printf("Erro na remoção da memoria compartilhada.\n");
  }

  wait(&state);
  return 0;
}