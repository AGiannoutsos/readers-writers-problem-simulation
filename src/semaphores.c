#include "semaphores.h"


int SemInit(key_t key, int numSems){

    int semId;

    semId = semget(key, numSems, 0666 | IPC_CREAT);

    union semun sem_union;
	sem_union.val = 1;

	for (int i = 0; i < numSems; i++){
		if (semctl(semId, i, SETVAL, sem_union) == -1){
			printf(" Error semaphore init failed %d\n ",numSems);
			return 0;
		}
	}
	
		
    return semId;
}

int SemDel(int semId, int numSems){

    union semun sem_union;
	semctl(semId, 0, IPC_RMID, sem_union);
	
}

int SemUp(int semId, int semNum){

    struct sembuf sem_b;

	sem_b.sem_num = semNum;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = 0;

	if (semop(semId, &sem_b, 1) == -1) {
		printf(" Error semaphore up %d\n",semNum);
		return(0);
	}
	return(1);
}

int SemDown(int semId, int semNum){
    
    struct sembuf sem_b;

	sem_b.sem_num = semNum;
	sem_b.sem_op = -1; /* V() */
	sem_b.sem_flg = 0;

	if (semop(semId, &sem_b, 1) == -1) {
		printf(" Error semaphore down %d \n",semNum);
		return(0);
	}
	return(1);
}