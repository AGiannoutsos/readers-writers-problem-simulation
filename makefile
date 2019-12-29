mainexe: main.c semaphores.c shmemory.c
	gcc main.c semaphores.c shmemory.c -o mainexe -lm

tester: tester.c
	gcc tester.c -o tester

clean:
	rm *.o
	rm mainexe
