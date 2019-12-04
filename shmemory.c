#include  "shmemory.h"

int ShmInit(key_t key, int size){
    int shmId;
    if((shmId = shmget (key, size, IPC_CREAT | 0666)) == -1) return -1;
    return shmId;
}

ShmData* ShmAt(int shmId){
    return (ShmData*)shmat (shmId, 0, 0);
}

int ShmDt(ShmData* shmPtr){
    if(shmdt(shmPtr) == -1){
        printf(" Error memory didnt detach \n");
        return 0;
    }
    return 1;
}

int ShmDea(int shmId){
    if(shmctl(shmId, IPC_RMID, 0) == -1){
        printf(" Error memory didnt deallocate \n");
        return 0;
    }
    return 1;
}


