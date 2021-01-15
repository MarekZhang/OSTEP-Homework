# Chapter7-Scheduling

Q1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.

SJF与FIFO的turnaround，response time相同； $T_{turnaround} = (200 + 400 + 600) / 3 = 400$; $T_{response} = (0 + 200 + 400) / 3 = 200$ 

```bash
./scheduler.py -p SJF -l 200,200,200 -c
./scheduler.py -p FIFO -l 200,200,200 -c
```

Q2. Now do the same but with jobs of different lengths: 100, 200, and 300.

SJF与FIFO相同 turnaround:333.33 | response:133.33

```bash
./scheduler.py -p SJF -l 100,200,300 -c
./scheduler.py -p FIFO -l 100,200,300 -c
```

Q3. Now do the same, but also with the RR scheduler and a time-slice of 1.

Round Robin

![Chapter7-Scheduling%20bc588572b0874a59898df705b8a8f466/IMG_0014D84A5113-1.jpeg](Chapter7-Scheduling%20bc588572b0874a59898df705b8a8f466/IMG_0014D84A5113-1.jpeg)

- turaround: (298 + 499 + 600) / 3 = 465.67 | response (0 + 1 + 2) / 3 = 1

    ```bash
    ./scheduler.py -p RR -q 1 -l 100,200,300 -c
    ```

Q4. For what types of workloads does SJF deliver the same turnaround times as FIFO?

the length of jobs in increasing order

Q5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?

the length first N - 1 jobs equals to time slice

Q6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?

response time increases

Q7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?

$\sum_1^n min\{len(job_i), T_{timeslice}\}$