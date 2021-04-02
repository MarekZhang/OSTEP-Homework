# Chapter 31 - Semaphores

Q1. The first problem is just to implement and test a solution to the fork/join problem, as described in the text. Even though this solution is described in the text, the act of typing it in on your own is worthwhile; even Bach would rewrite Vivaldi, allowing one soon-to-be master to learn from an existing one. See `fork-join.c` for details. Add the call `sleep(1)` to the child to ensure it is working.

```c
// sem_open on Mac
s = sem_open("/sem", O_CREAT, S_IRWXU, 0);
```

Q2.Let’s now generalize this a bit by investigating the rendezvous problem. The problem is as follows: you have two threads, each of which are about to enter the rendezvous point in the code. Neither should exit this part of the code before the other enters it. Consider using two semaphores for this task, and see `rendezvous.c` for details.

```bash
$ gcc -o rendezvous rendezvous.c -Wall -pthread

$ ./rendezvous
parent: begin
child 2: before
child 1: before
child 1: after
child 2: after
parent: end

```

Q3. Now go one step further by implementing a general solution to barrier synchronization. Assume there are two points in a sequential piece of code, called P1 and P2. Putting a barrier between P1 and P2 guarantees that all threads will execute P1 before any one thread executes P2. Your task: write the code to implement a `barrier()` function that can be used in this manner. It is safe to assume you know N (the total number of threads in the running program) and that all N threads will try to enter the barrier. Again, you should likely use two semaphores to achieve the solution, and some other integers to count things. See `barrier.c` for details.

```bash
$ gcc -o barrier barrier.c -Wall -pthread -g

$ ./barrier 5
parent: begin
child 0: before
child 2: before
child 1: before
child 4: before
child 3: before
child 0: after
child 2: after
child 1: after
child 4: after
child 3: after
parent: end
```

Q4. Now let’s solve the reader-writer problem, also as described in the text. In this first take, don’t worry about starvation. See the code in `reader-writer.c` for details. Add sleep() calls to your code to demonstrate it works as you expect. Can you show the existence of the starvation problem?

```bash
$ gcc -o reader-writer reader-writer.c -Wall -pthread -g
$ ./reader-writer 4 2 3
begin
read 0
read 0
read 0
read 0
read 0
read 0
read 0
read 0
read 0
read 0
read 0
read 0
write 1
write 2
write 3
write 4
write 5
write 6
end: value 6
```

Q5. Let’s look at the reader-writer problem again, but this time, worry about starvation. How can you ensure that all readers and writers eventually make progress? See `reader-writer-nostarve.c` for details.

Q6. Use semaphores to build a no-starve mutex, in which any thread that tries to acquire the mutex will eventually obtain it. See the code in `mutex-nostarve.c` for more information.

- [The little book of semaphores](https://greenteapress.com/wp/semaphores/) Morris's solution: using two turnstiles