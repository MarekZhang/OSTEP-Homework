# Chapter 30 - Condition Variables

Q4. Let’s look at some timings. How long do you think the following execution, with one producer, three consumers, a single-entry shared buffer, and each consumer pausing at point c3 for a second, will take?

`./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,1,0,0,0:0,0,0,1,0,0,0:0,0,0,1,0,0,0 -l 10 -v -t`

- number_of_c3 seconds

Q5. Now change the size of the shared buffer to 3 (-m 3). Will this make any difference in the total time?

- no. Because the number of loops does not change

Q6. Now change the location of the sleep to c6 (this models a consumer taking something off the queue and then doing something with it), again using a single-entry buffer. What time do you predict in this case?

`./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,0,0,0,1:0,0,0,0,0,0,1:0,0,0,0,0,0,1 -l 10 -v -t`

- number_of_c3 / 3 seconds

Q7. Finally, change the buffer size to 3 again (-m 3). What time do you predict now?

- no changes

Q8. Now let’s look at main-one-cv-while.c. Can you configure a sleep string, assuming a single producer, one consumer, and a buffer of size 1, to cause a problem with this code?

- no

Q9. Now change the number of consumers to two. Can you construct sleep strings for the producer and the consumers so as to cause a problem in the code?

```bash
$ ./main-one-cv-while -l 10 -p 1 -c 2 -m 1 -C 0,0,0,0,0,0,0:0,0,0,0,1,0,0, -v
```

Q10. Now examine `main-two-cvs-if.c`. Can you cause a problem to happen in this code? Again consider the case where there is only one consumer, and then the case where there is more than one.

```bash
$ ./main-two-cvs-if -p 1 -c 2 -l 10 -m 1 -C 0,0,0,0,0,0,0:0,0,0,0,2,0,0
//error: tried to get an empty buffer
```

Q11. Finally, examine main-two-cvs-while-extra-unlock.c. What problem arises when you release the lock before doing a put or a get? Can you reliably cause such a problem to happen, given the sleep strings? What bad thing can happen?

```bash
$ ./main-two-cvs-while-extra-unlock -p 1 -c 2 -m 1 -l 10 -C 0,0,0,0,0,0,0:0,0,0,0,2,0,0 -v
```

- consumer_0 consumes zero value