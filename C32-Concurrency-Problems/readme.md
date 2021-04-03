# Chapter 32 - Concurrency Problems

Q2. Now add the -d flag, and change the number of loops(-l) from 1 to higher numbers. What happens? Does the code (always) deadlock?

`$ ./vector-deadlock -2 3 -l 10000 -v -d`

Q3. How does changing the number of threads (-n) change the outcome of the program? Are there any values of -n that ensure no deadlock occurs?

- n equals to 1 which means only 1 thread accessing the vector

Q4. Now examine the code in `vector-global-order.c`. First, make sure you understand what the code is trying to do; do you understand why the code avoids deadlock? Also, why is there a special case in this vector_add() routine when the source and destination vectors are the same?

- enforce lock ordering by lock address
- we cannot lock the same lock twice

Q5. Now run the code with the following flags: `-t -n 2 -l 100000 -d`. How long does the code take to complete? How does the total time change when you increase the number of loops, or the number of threads?

```bash
$ ./vector-global-order -t -n 2 -l 200000 -d
Time: 0.03 seconds
```

Q6. What happens if you turn on the parallelism flag (-p)? How much would you expect performance to change when each thread is working on adding different vectors (which is what -p enables) versus working on the same ones?

```bash
$ ./vector-global-order -t -n 2 -l 10000000 -d
Time: 2.08 seconds

$ ./vector-global-order -t -n 2 -l 10000000 -d -p
Time: 0.89 seconds
```

Q7. Now let’s study `vector-try-wait.c` First make sure you understand the code. Is the first call to pthread_mutex_trylock() really needed? Now run the code. How fast does it run compared to the global order approach? How does the number of retries, as counted by the code, change as the number of threads increases?

```bash
$ ./vector-global-order -t -n 2 -l 10000000 -d
Time: 2.08 second

$ ./vector-try-wait -t -n 2 -l 10000000
Retries: 418525
Time: 3.85 seconds
```

Q8. Now let’s look at `vector-avoid-hold-and-wait.c`. What is the main problem with this approach? How does its performance compare to the other versions, when running both with -p and without it?

```bash
$ ./vector-avoid-hold-and-wait -t -n 2 -l 10000000
Time: 3.67 seconds
```

- global lock worse the performance

Q9. Now compare its performance to the other versions, both when threads are working on the same two vectors (no -p) and when each thread is working on separate vectors (-p). How does this no-lock version perform?

```bash
$ ./vector-nolock -t -n 2 -l 10000000
Time: 13.66 seconds

$ ./vector-nolock -t -n 2 -l 10000000 -p
Time: 5.50 seconds
```