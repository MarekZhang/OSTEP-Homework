# Chapter 29 - Lock-based Concurrent Data Structures

Q2. Now, build a simple concurrent counter and measure how long it takes to increment the counter many times as the number of threads increases. How many CPUs are available on the system you are using? Does this number impact your measurements at all?

```bash
// This experiment was run upon a MacBook Pro with 2.3 GHz Quad-Core Intel Core i5
$ gcc -o concurrent concurrent_counter.c
$ ./concurrent
Running 1 threads..
counter value: 1000000
time elapsed 0.02 s

Running 2 threads..
counter value: 2000000
time elapsed 0.09 s

Running 3 threads..
counter value: 3000000
time elapsed 0.20 s

Running 4 threads..
counter value: 4000000
time elapsed 0.29 s
```

Q3. Next, build a version of the sloppy counter. Once again, measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?

```bash
// This experiment was run upon a MacBook Pro with 2.3 GHz Quad-Core Intel Core i5
$ gcc -o scalable scalable_counter.c -Wall
$ ./scalable
Time elapsed: 0.62s | Approximate Factor:    1 | Global counter : 4000000

Time elapsed: 0.47s | Approximate Factor:    2 | Global counter : 4000000

Time elapsed: 0.40s | Approximate Factor:    4 | Global counter : 4000000

Time elapsed: 0.38s | Approximate Factor:    8 | Global counter : 4000000

Time elapsed: 0.34s | Approximate Factor:   16 | Global counter : 4000000

Time elapsed: 0.34s | Approximate Factor:   32 | Global counter : 4000000

Time elapsed: 0.33s | Approximate Factor:   64 | Global counter : 4000000

Time elapsed: 0.33s | Approximate Factor:  128 | Global counter : 4000000

Time elapsed: 0.33s | Approximate Factor:  256 | Global counter : 4000000

Time elapsed: 0.30s | Approximate Factor:  512 | Global counter : 3999744

Time elapsed: 0.32s | Approximate Factor: 1024 | Global counter : 3999744
```

Q4. Build a version of a linked list that uses hand-over-hand lockingMS04, as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?

```bash
$ gcc -o hand hand_over_hand_linked_list.c -Wall -g
$ ./hand 500000
4 threads | each inserting 500000 elements to the list | time elapsed 0.73s
4 threads | traversing the list | time elapsed 0.06s
The length of the linked list is 2000000
```

- I did not figure out how to insert nodes into the linked list using hand-over-hand lock, so I used a global lock to replace it
- the performance of lock-coupling traverse is good