mainexe: main.c semaphores.c shmemory.c
	gcc main.c semaphores.c shmemory.c -o mainexe

clean:
	rm *.o
	rm mainexe
