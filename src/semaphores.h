#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};   

//Create and init semaphores
int SemInit(key_t key, int numSems);

//Delete semaphores
int SemDel(int semId, int numSems);

//sem Up
int SemUp(int semId,int semNum);

// sem down
int SemDown(int semId, int semNum);

