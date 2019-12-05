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
    ShmData* shmData; int shmId;
    pid_t pid;
    int readId, writeId, semId;

    if (argc != 4){
        printf(" Error wrong input \n");
        //return 0;
    }
    peers   = atoi(argv[--argc]);
    rwRatio = atoi(argv[--argc]) -1; //for inputs between (1 and 100) -> (1% - 100%)
    shmSize = atoi(argv[--argc]);
    
    srand(getpid());

    printf("%d %d\n", rwRatio,shmSize+5);

    // initialize the shared matrix
    shmId = ShmInit((key_t)1, shmSize);
    shmData = ShmAt(shmId);
    for(int i=0; i<shmSize; i++){
        shmData[i].avTime = 0;
        shmData[i].writes = 0;
        shmData[i].reads = 0;
    }

    // initialize array for clild pid
    pid_t childPid[peers];

    // initialize semaphores
    semId   = SemInit((key_t)2000, 1);
    readId  = SemInit((key_t)3000, shmSize);
    writeId = SemInit((key_t)4000, shmSize);


    for (int i = 0; i < peers; i++){
        pid = fork();
        if (pid == 0) //child process
            break;
        childPid[i] = pid;
    }
    

    if (pid != 0){ //if parent 

    }
    else{ // if child
        srand(getpid());
        int reads   = 0;
        int writes  = 0;
        int entry   = 0;

        for (int i = 0; i < 100000; i++){ // Repeat r/w process 10 times
        
            if (rand()%100 > rwRatio){ //Writer

                entry = rand()%shmSize;  //Get a random shared value

                SemDown(writeId, 0);
                shmData[entry].writes++;
                SemUp(writeId, 0);

            } else { //Reader
                
            }
        }
        
    }

/*  
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
            shmData[0].writes++;
            SemUp(semId,0);
        }   
        else{
            SemDown(semId,0);
            shmData[0].writes--;
            SemUp(semId,0);
        }
            
    }
*/
    if (pid != 0){ //terminate child processes
        
        
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
        int sum=0;
        for (int i = 0; i < shmSize; i++){
            printf("shared = %d\n",shmData[i].writes);
            sum += shmData[i].writes;
        }
        printf("suuum -> %d\n",sum);
		
		exit(0);
	}

    
    




    ShmDt(shmData);
    ShmDea(shmId);




}