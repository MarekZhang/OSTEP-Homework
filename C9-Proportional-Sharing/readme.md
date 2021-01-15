# Chapter 9 - Proportional Share

Q1. Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.

```c
$ ./lottery.py -j 3 -s 1 -c
$ ./lottery.py -j 3 -s 2 -c
$ ./lottery.py -j 3 -s 3 -c
```

Q2. Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., `-l 10:1,10:100`). What happens when the number of tickets is so imbalanced? Will job 0 ever run before job 1 completes? How often? In general, what does such a ticket imbalance do to the behavior of lottery scheduling?

```c
$ ./lottery.py -l 10:1,10:100 -c
```

- the probability of running job 1 is 100 times of job 0

Q3. When running with two jobs of length 100 and equal ticket allocations of 100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.

```c
$ ./lottery.py -l 100:100,100:100 -s 0 -c
U = 192 / 200 = 0.96

$ ./lottery.py -l 100:100,100:100 -s 1 -c
U = 196 / 200 = 0.98

$ ./lottery.py -l 100:100,100:100 -s 2 -c
U = 190 / 200 = 0.95
```

Q4, Q5 refer to @xxyzz [soltution](https://github.com/xxyzz/ostep-hw/tree/master/9)