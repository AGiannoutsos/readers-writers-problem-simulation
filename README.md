# readers-writers-problem-simulation
A simulation of the readers-writers problem with statistics 

This program simulates the behavior of readers and writers in the operating system with the use of semaphores

## For compilation and execution

```make mainexe```

The executable is in the build folder

```./build/mainexe <arg1> <arg2> <arg3>```

```<arg1>``` = total number of peers readers + writers

```<arg2>``` = percentage of readers for [1 to 100] for example 30 means 30% readers and 70% writers

```<arg3>``` = size of shared memory that peers try to access to

An ideal execution could be:

```./build/mainexe 100 70 100```

## About the simulation
The program tries to emulate the behavior of readers and writers.
There is a shared memory in which different processes try to have access to.
After the initialization of the semaphores and the shared memory the program forks <arg1> number of processes.
Each process has an <arg2> probability of being a reader or a writer.
 
 
## Results
The simulation runs for a single case scenario. However, there is a simple tester.c program that runs the simulations with different combinations of peers and memory size.
```make tester```
```
For memory from [0,100]
  For peers form [100,0]
      ./exemain peers <ratio> memory
```
#### Graphic Results

The below graph shows the sum of the waiting time of all the peers in relation to the memory size and the number of peers with a 50% ratio of readers and writers


Z-axis is the total waiting time
X and Y axes are the memory size and the number of peers
 

This chart is a projection of the above chart on the X-axis

Now, this axis is the size of memory from 1 to 100 memory slots. We can observe the periodicity of the number of peers which is a result of the for loop. The dropping points are when peers are very few.

After excluding the extreme cases where peers are relative very few in relation to memory size, memory plays a more important role in waiting time.

##### Conclusion

From the chart, we can clearly come to the conclusion that waiting time depends more on memory size rather than peers' numbers.

However, after a point adding more memory does not change the time drastically. The curve follows an exponential rate.

For that reason, in association with real applications, we can see that resources are important for decreasing the waiting times, although, adding too many may have the expected results and raise the costs superfluously.
