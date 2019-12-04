#include "semaphores.h"


int SemInit(key_t key, int numSems){

    int semId;

    semId = semget(key, numSems, 0666 | IPC_CREAT);

    union semun sem_union;
	sem_union.val = 1;
	if (semctl(semId, 0, SETVAL, sem_union) == -1){
        printf(" Error semaphore init failed\n ");
        return 0;
    }
    return semId;
}

int SemDel(int semId){

    union semun sem_union;

	if (semctl(semId, 0, IPC_RMID, sem_union) == -1)
	printf(" Error semaphore didnt close \n");
}

int SemUp(int semId, int semNum){

    struct sembuf sem_b;

	sem_b.sem_num = semNum;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;

	if (semop(semId, &sem_b, 1) == -1) {
		printf(" Error semaphore up \n");
		return(0);
	}
	return(1);
}

int SemDown(int semId, int semNum){
    
    struct sembuf sem_b;

	sem_b.sem_num = semNum;
	sem_b.sem_op = -1; /* V() */
	sem_b.sem_flg = SEM_UNDO;

	if (semop(semId, &sem_b, 1) == -1) {
		printf(" Error semaphore up \n");
		return(0);
	}
	return(1);
}