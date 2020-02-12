# readers-writers-problem-simulation
A simulation of the readers-writers problem with statistics 

This program simulates the behaviour of readers and writers in the operating system with the use of semaphores

## For compilation and excecuion

```make mainexe```

The excecutable is in the build folder

```./build/mainexe <arg1> <arg2> <arg3>```

```<arg1>``` = total number of peers readers + writers

```<arg2>``` = percentage of readers for [1 to 100] for example 30 means 30% readers and 70% writers

```<arg3>``` = size of shared memory that peers try to access to

An ideal excecution could be:

```./build/mainexe 100 70 100```

## About the simulation
The program tries to emulate the behaviour of readers and writers.
There is a shared memory in wich different proceses try to have access to.
After the initialization of the semaphores and the shared memory the program forks <arg1> number of processes.
Each process has an <arg2> propability of being a reader or a writer.

 
