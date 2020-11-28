#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define TRUE  1
#define FALSE 0


// Implementa um timer com o Signal Alarm

int timeout = FALSE;

void timer(){
  timeout = TRUE;
}

// Inicia o timer

void set_timer(int time){
  signal(SIGALRM, timer);
  timeout = FALSE;
  alarm(time);
}

// Encerra o timer

void stop_timer(){
  alarm(0);
  timeout = FALSE;
}

// Retorna se ocorreu um timeout 

int is_timeout(){
  return timeout;
}                                                                                                                                                                                                                                                                                                                                                                                           

int main(){
  int pid, idfila, estado;

  // Estrutura da mensagem

  struct mensagem{
    long pid;
    int numero;
  };

  struct mensagem mensagem_env, mensagem_rec;

  // Cria a mensagem
  if ((idfila = msgget(0x1223, IPC_CREAT|0x1B6)) < 0){
    printf("erro na criacao da fila\n");
    exit(1);
  }

  if ((pid = fork()) < 0){
    printf("erro no fork\n");
    exit(1);
  }

  if (pid == 0){                                                                                                  
    // Processo filho (P1)
    for (int i = 1; i <= 10; i++){
      // Configura um timer de 2 segundos                                                                                                                                             
      set_timer(2);
      // Aguarda até que a mensagem seja recebida ou ocorra um timeout
      while(!is_timeout()){
        if ((msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec), 0, 0) != -1) &&
        (mensagem_rec.numero == i)){
          stop_timer();
          // Encerra o timer se a mensagem for recebida antes dos 2s
          printf("recebi mensagem pid=%ld numero=%d\n", mensagem_rec.pid, mensagem_rec.numero);
          break;
        }
      }
      if (is_timeout()){
        printf("ocorreu timeout - nao recebi mensagem em 2 s\n");
      }
    }
    exit(0);
  }
  // Processo pai (P2)

  srand( (unsigned)time(NULL) );
  mensagem_env.pid = getpid();
  
  for (int i = 1; i <= 10; i++){
    // Dorme por um tempo aleatório entre 1 e 4 segundos
    int time = 1 + ( rand() % 4 );
    sleep(time);
    // Envia a mensagem
    mensagem_env.numero = i;
    msgsnd(idfila, &mensagem_env, sizeof(mensagem_env), 0);
    printf("mensagem pid=%ld numero=%d enviada\n", mensagem_env.pid, i);
  }

  // Espera o processo filho(P1)
  wait(&estado);

  // Remove a fila
  msgctl(idfila, IPC_RMID, NULL);
  exit(0);

  return 0;
}