#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

#include "semaphores.h"
#include "shmemory.h"

int main(int argc,char **argv){

    int rwRatio, shmSize, peers;
    ShmData* data; int shmId;
    pid_t pid;

    if (argc != 4){
        printf(" Error wrong input \n");
        //return 0;
    }
    peers   = atoi(argv[--argc]);
    rwRatio = atoi(argv[--argc]);
    shmSize = atoi(argv[--argc]);
    
    srand(getpid());

    printf("%d %d\n", rwRatio,shmSize+5);

    // initialize the shared matrix
    shmId = ShmInit((key_t)1234, shmSize);
    data = ShmAt(shmId);
    for(int i=0; i<shmSize; i++){
        data[i].avTime = 0;
        data[i].writes = 0;
        data[i].reads = 0;
    }

    // initialize array for clild pid
    pid_t childPid[peers];

    // initialize semaphores
    int semId = SemInit((key_t)12345, 1);

    for (int i = 0; i < peers; i++){
        pid = fork();
        if (pid == 0) //child process
            break;
        childPid[i] = pid;
    }
    

    if (pid != 0){ //if parent 

    }
    else{ // if child

    }

    
    int n;
    switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			printf("This is the child\n");
      printf("mySem of child %d\n",pid);
			n = 30000;
			break;
		default:
			n = 30100;
			break;
	}

    for (int i = 0; i < n; i++){
        if(pid != 0){
            SemDown(semId,0);
            data[0].writes++;
            SemUp(semId,0);
        }   
        else{
            SemDown(semId,0);
            data[0].writes--;
            SemUp(semId,0);
        }
            
    }

    if (pid != 0){ //terminate child processes
        printf("shared = %d\n",data[0].writes);
        int status;
        pid_t childTerminatedPid;

		// close all child processes
        for (int i = 0; i < peers; i++){
            childTerminatedPid = waitpid(childPid[i], &status, 0);
            printf("Child has finished: PID = %d\n", childTerminatedPid);
            if(WIFEXITED(status))
                printf("Child exited with code %d\n", WEXITSTATUS(status));
            else
                printf("Child terminated abnormally\n");
        }
        
		
		exit(0);
	}

    
    




    ShmDt(data);
    ShmDea(shmId);




}