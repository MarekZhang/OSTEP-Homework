# Chapter8-MLFQ

**Q1.** Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.

```bash
$ ./mlfq.py -n 2 -j 2 -M 0 -c

Job List:
  Job  0: startTime   0 - runTime  84 - ioFreq   0
  Job  1: startTime   0 - runTime  42 - ioFreq   0
```

turnaround: (92 + 126) / 2 = 109 | response: (0 + 10) / 2 = 5

**Q2** How would you run the scheduler to reproduce each of the exam- ples in the chapter?

```bash
//figure 8.2
$ ./mlfq.py -n 3 -l 0,200,0 -q 10 -c

//figure 8.3
$ ./mlfq.py -n 3 -l 0,200,0:100,20,0 -q 10 -c

//figure 8.4
$ ./mlfq.py -n 3 -l 0,200,0:50,25,1 -q 10 -i 4 -c

//figure 8.5
$ ./mlfq.py -n 3 -l 0,200,0:50,20,3:53,20,3 -q 10 -i 3 -c
$ ./mlfq.py -n 3 -l 0,200,0:50,20,3:53,20,3 -q 10 -i 3 -c -B 30 -S

//figure 8.6
$ ./mlfq.py -n 3 -l 0,200,0:50,100,9 -i 1 -S -c
$ ./mlfq.py -n 3 -l 0,200,0:50,100,9 -i 1 -c
```

**Q3**. How would you configure the scheduler parameters to behave just like a round-robin scheduler?

time slice ≤ len(min Job) / num_of_jobs

**Q4.** Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval. 

```bash
$ ./mlfq.py -n 3 -l 0,200,0:50,99,9 -i 1 -S -c
```

**Q5.** Given a system with a quantum length of 10 ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long- running (and potentially-starving) job gets at least 5% of the CPU?

$T_{boost} = 10 / 5\% = 200ms$ 

**Q6**. One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.

```bash
$ ./mlfq.py -n 2 -q 10 -l 0,50,0:0,50,11 -i 1 -S -c
$ ./mlfq.py -n 2 -q 10 -l 0,50,0:0,50,11 -i 1 -S -I -c
```

without `-I` job that just finished I/O is inserted to the end of the queue