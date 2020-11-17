#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/sem.h>

#define SEM_READ_KEY 0x1232
#define SEM_WRITE_KEY 0x1233
#define SHM_KEY 0x1234

struct sembuf operacao[2];

int p_sem(int idsem){
	operacao[0].sem_num = 0;
	operacao[0].sem_op = 0;
	operacao[0].sem_flg = 0;
	operacao[1].sem_num = 0;
	operacao[1].sem_op = 1;
	operacao[1].sem_flg = 0;
	if ( semop(idsem, operacao, 2) < 0) printf("Erro\n");
}

int v_sem(int idsem){
	operacao[0].sem_num = 0;
	operacao[0].sem_op = -1;
	operacao[0].sem_flg = 0;
	if ( semop(idsem, operacao, 1) < 0) printf("Erro\n");
 } 

void process1(){
	int idshm, idsem_write, idsem_read;

	/* Obtendo semáforos */ 
	if ((idsem_write = semget(SEM_WRITE_KEY, 1, IPC_CREAT|0x1ff)) < 0){ 
		printf("erro na criacao do semaforo\n");
		exit(1);
	} 
	if ((idsem_read = semget(SEM_READ_KEY, 1, IPC_CREAT|0x1ff)) < 0){ 
		printf("erro na criacao do semaforo\n");
		exit(1);
	} 

	/* Leitura bloqueada inicialmente */
	p_sem(idsem_read);
	for(int i=1;i<=20;i++){
		/* Semáforo de escrita ocupado */
		p_sem(idsem_write);
		if ((idshm = shmget(SHM_KEY, sizeof(int),IPC_CREAT|0x1ff)) < 0){
 			printf("erro na criacao da memória compartilhada\n");
 			exit(1);
		}
		int* pshm = (int *) shmat(idshm, (char *)0, 0);
 		if (pshm == (int *)-1){
 			printf("erro no attach\n"); 
 			exit(1);		
 		}
 		*pshm = i; 
		printf("processo %d escreveu %d\n", getpid(),i);	

		/* Liberando para a leitura */
		v_sem(idsem_read);
	}
}

void process2(){
	int idshm, idsem_write, idsem_read;
	
	/* Obtendo semáforos */ 
	if ((idsem_write = semget(SEM_WRITE_KEY, 1, IPC_CREAT|0x1ff)) < 0){ 
		printf("erro na criacao do semaforo\n");
		exit(1);
	} 
	if ((idsem_read = semget(SEM_READ_KEY, 1, IPC_CREAT|0x1ff)) < 0){ 
		printf("erro na criacao do semaforo\n");
		exit(1);
	} 

	for(int i=1;i<=20;i++){

		/* Bloqueando até escrita estar concluída */
		p_sem(idsem_read);
		if ((idshm = shmget(SHM_KEY, sizeof(int),IPC_CREAT|0x1ff)) < 0){
 			printf("erro na criacao da memória compartilhada\n");
 			exit(1);
		}
		int* pshm = (int *) shmat(idshm, (char *)0, 0);
 		if (pshm == (int *)-1){
 			printf("erro no attach\n"); 
 			exit(1);		
 		}
		printf("processo %d leu %d\n", getpid(),*pshm);	

		/* Liberando escrita */
		v_sem(idsem_write);
	}

	/* Exclusão da memória e semáforos */ 
	if(shmctl(idshm, IPC_RMID, NULL) < 0){
    	printf("Erro na exclusão da memoria compartilhada\n");
  	}
	if(semctl(idsem_read, 0, IPC_RMID, NULL) == -1){
		printf("Erro na exclusão do semaforo de leitura\n");
	}
	if(semctl(idsem_write, 0, IPC_RMID, NULL) == -1){
	    printf("Erro na exclusão do semaforo de escrita\n");
	}
}


int main(int argc, char* argv[]){

	int pid;

	pid = fork();
	/* P1 = PAI (ESCRITA) */
	if(pid != 0){
		process1();
		exit(0);
	}

	/* P2 = FILHO (LEITURA)*/
	else{
		process2();
		exit(0);
	}
	return 0;
}