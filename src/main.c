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
#include <math.h>
#include <sys/time.h>

#include "semaphores.h"
#include "shmemory.h"

#define lamda 100
#define LOOPS 10

#define ln(x) log(x)
#define U(x) (((double)(x + 1)/10))
#define expTime(x) (-(ln(U(x)) * 1000000))




int main(int argc,char **argv){

    int rwRatio, shmSize, peers, maxReaders = 3;
    ShmData* shmData; int shmId, shmStatId;
    int readersCounterId; int* readersCounterData;
    long* statData;
    pid_t pid;
    int readId, writeId, semId, mutexId, counterId, printId;

    if (argc != 4){
        printf(" Error wrong input %d\n",argc);
        //return 0;
    }
    shmSize = atoi(argv[--argc]);
    rwRatio = atoi(argv[--argc]) -1; //for inputs between (1 and 100) -> (1% - 100%)
    peers   = atoi(argv[--argc]);
    srand(getpid());

    // initialize the shared matrix
    shmId   = ShmInit((key_t)187, shmSize*sizeof(ShmData));
    shmData = ShmAt(shmId);
    for(int i=0; i<shmSize; i++){
        shmData[i].avTime = 0;
        shmData[i].writes = 0;
        shmData[i].reads = 0;
    }
    // initialize shared vounter for readers
    readersCounterId    = ShmInit((key_t)6000, shmSize*sizeof(int));
    readersCounterData  = (int*)shmat (readersCounterId, 0, 0);
    for(int i=0; i<shmSize; i++){
        readersCounterData[i]  = 0;
    }

    // initialize shared memory to keep stats
    // 0 writing waiting time | 1 reading w t | 2 total waiting time
    shmStatId    = ShmInit((key_t)6123, 3*sizeof(long));
    statData  = (long*)shmat (shmStatId, 0, 0);
    for(int i=0; i<3; i++){
        statData[i]  = 0;
    }
    

    // initialize array for clild pid
    pid_t childPid[peers];

    // initialize semaphores
    readId      = SemInit((key_t)31003, shmSize);
    writeId     = SemInit((key_t)40100, shmSize);
    counterId   = SemInit((key_t)5760800, shmSize);
    printId     = SemInit((key_t)2340, 1);

    // initialize counters for statistics
    struct timeval stop, start;
    double avgTime         = 0;
    double avgTimeWriter   = 0;
    double avgTimeReader   = 0;
    long totalTimeReader   = 0;
    long totalTimeWriter   = 0;
    long currentTime = 0;
    long totalTime   = 0;
    int reads   = 0; 
    int writes  = 0;
    int entry   = 0;

    // File to print statistics
    FILE *logFile = fopen( "logfile.txt", "w+");

    FILE *logStat = fopen( "logstat.txt", "a");

    for (int i = 0; i < peers; i++){
        pid = fork();
        if (pid == 0) //child process
            break;
        childPid[i] = pid;
    }
    
    // r/w processes
    if (pid != 0){ //if parent 
        printf("Parent with pid: %d \n",(pid));
    }
    else{ // if child
        printf("Child with pid: %d \n",getpid());
        srand(getpid());
        // counter for average time in microseconds
        

        for (int i = 0; i < LOOPS; i++){ // Repeat r/w process 10 times
            if (rand()%100 > rwRatio){ //Writer

                entry = rand()%shmSize;  //Get a random shared value
                writes++;
                gettimeofday(&start, NULL);

                // ********Critical section of writer******** //
                SemDown(writeId, entry);
                
                gettimeofday(&stop, NULL); // Waiting time
                currentTime = (stop.tv_sec - start.tv_sec)*1000000 + stop.tv_usec - start.tv_usec;
                usleep( expTime(rand()%10)/lamda ); // wait expodential time
                shmData[entry].writes++;

                SemUp(writeId, entry);

                totalTime       += currentTime;
                totalTimeWriter += currentTime;

            } else { //Reader

                entry = rand()%shmSize;  //Get a random shared value
                reads++;

                
                gettimeofday(&start, NULL);
                //update counter
                SemDown(counterId, entry);
                readersCounterData[entry]++;
                if (readersCounterData[entry] == 1){  SemDown(writeId, entry); }
                SemUp(counterId, entry);
                
                // ********critical section of reader******** //
                SemDown(readId, entry);

                gettimeofday(&stop, NULL);
                currentTime = (stop.tv_sec - start.tv_sec)*1000000 + stop.tv_usec - start.tv_usec;
                usleep( expTime(rand()%10)/lamda ); // wait expodential time
                shmData[entry].reads++;

                SemUp(readId, entry);

                //update counter
                SemDown(counterId, entry);
                readersCounterData[entry]--;
                if (readersCounterData[entry] == 0)  SemUp(writeId, entry);
                SemUp(counterId, entry);

                totalTime       += currentTime;
                totalTimeReader += currentTime;
                
            }
        }

        
        
    }

    if (pid == 0){ //Printig statistics

    if(writes) avgTimeWriter = totalTimeWriter / writes;
    if(reads) avgTimeReader = totalTimeReader / reads;
    if(reads + writes) avgTime = totalTime / (reads + writes);

    SemDown(printId, 0);
    fprintf(logFile,"Child with pid: %d\n", getpid());
    fprintf(logFile,"Total write time: %ld  average read  time: %f\n", totalTimeWriter, avgTimeWriter);
    fprintf(logFile,"Total read  time: %ld  average write time: %f\n", totalTimeReader, avgTimeReader);
    fprintf(logFile,"Total       time: %ld  average total time: %f\n", totalTime, avgTime);
    fprintf(logFile,"\n");
    //shared statistics
    statData[0] += totalTimeWriter/10;
    statData[1] += totalTimeReader/10;
    statData[2] += totalTime/10;

    SemUp(printId, 0);
    }

    // exit processes
    if (pid != 0){ //terminate child processes
        
        int status;
        pid_t childTerminatedPid;

		// close all child processes
        for (int i = 0; i < peers; i++){
            childTerminatedPid = waitpid(childPid[i], &status, 0);
            printf("Child has finished: PID = %d\n", childTerminatedPid);
            if(WIFEXITED(status))
                printf("Child exited with code %d\n", status);
            else
                printf("Child terminated abnormally %d\n",(status));
        }

        // Validate results
        int sumReaders = 0;
        int sumWriters = 0;

        SemDown(printId, 0);
        fprintf(logFile,"\n     ***VALIDATING RESULTS*** \n\n");
        for (int i = 0; i < shmSize; i++){
            fprintf(logFile ,"Shared -> %d   Readers -> %d   Writers -> %d\n",i,shmData[i].reads, shmData[i].writes);
            sumReaders   += shmData[i].reads;
            sumWriters   += shmData[i].writes;
        }
        fprintf(logFile,"___________________________________________\n");
        fprintf(logFile,"Total +       Readers -> %d   Writers -> %d   Total -> %d\n",sumReaders, sumWriters, sumWriters + sumReaders);
        fprintf(logFile,"\nTotal must be peers * LOOPS which is -> %d\n",peers * LOOPS);
        if (peers * LOOPS == sumWriters, sumWriters + sumReaders) fprintf(logFile," Correct!!! \n");

        //Print statistics in logStat
        fprintf(logStat,"%ld %ld %ld\n",statData[2]/1000, statData[1]/1000, statData[0]/1000);

        SemUp(printId, 0);
	}else{       

        exit(0);
    }

    
    // Delete semaphores
    SemDel(readId, shmSize);
    SemDel(writeId, shmSize);
    SemDel(counterId, shmSize);
    SemDel(printId, 0);

    // Deallocate shared memory
    ShmDea(shmStatId);
    if(shmdt(statData) == -1){
        printf(" Error memory didnt detach aa\n");
        return 0;
    }

    ShmDea(readersCounterId);
    if(shmdt(readersCounterData) == -1){
        printf(" Error memory didnt detach aa\n");
        return 0;
    }

    ShmDea(shmId);
    ShmDt(shmData);
    
    fclose(logFile);
    fclose(logStat);



}