# Chapter 17 - Free-Space Management

Q1. First run with the flags `-n 10 -H 0 -p BEST -s 0` to generate a few random allocations and frees. Can you predict what alloc()/free() will return? Can you guess the state of the free list after each request? What do you notice about the free list over time?

```bash
$ ./malloc.py -n 10 -H 0 -p BEST -s 0 -c
```

physical memory is chopped into small chunks

Q2. How are the results different when using a WORST fit policy to search the freelist(`-p WORST`)? What changes?

```bash
$ ./malloc.py -n 10 -H 0 -p WORST -s 0 -c
```

generated more small chunks

Q3. What about when using FIRST fit (-p FIRST)? What speeds up when you use first fit?

```bash
$ ./malloc.py -n 10 -H 0 -p FIRST -s 0 -c
```

speed up searching

Q4. For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings(`-l ADDRSORT,-l SIZESORT+,-l SIZESORT-`) to see how the policies and the list orderings interact.

has no influence on best fit / worst fit strategy

```bash
$ ./malloc.py -n 10 -H 0 -p FIRST -l SIZESORT+ -s 0 -c
```

Q5. Coalescing of a free list can be quite important. Increase the number of random allocations (say to `-n 1000`). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the `-C` flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?

- cannot allocate space for larger allocation request(external fragmentation)
- sort by address is more efficient for coalescing

```bash
$ ./malloc.py -n 1000 -H 0 -p FIRST -s 0 -c
$ ./malloc.py -n 1000 -H 0 -p FIRST -s 0 -C -c
```

Q6. What happens when you change the percent allocated fraction -P to higher than 50? What happens to allocations as it nears 100? What about as the percent nears 0?

- when the percentage of allocation instruction is higher, higher possibility to receive unsuccessful allocation operation

Q7. What kind of specific requests can you make to generate a highly-fragmented free space? Use the `-A` flag to create fragmented free lists, and see how different policies and options change the organization of the free list.

...