#include <stdio.h> 
#include <sys/shm.h> 
#include <sys/types.h>
#include <stdlib.h>

typedef struct ShmData
{
    int reads;
    int writes;
    long double avTime;
} ShmData;


//initialize share memory
int ShmInit(key_t key, int size);

//return a pointer to memory
ShmData* ShmAt(int shmId);

// Detach memory
int ShmDt(ShmData* shmPtr);

//Deallocate memory
int ShmDea(int shmId);

