# Chapter 19 - TLBs

1. we use `gettimeofday()` as the timer, and the minimum unit is microsecond. We want to acquire result with the precision of nanosecond, so we need to repeat the measurement and calculate the average value. The default times of repetition is 1000 times, we can change the value in  `test.sh` and `data.sh`

    Refer to [@xxyz](https://github.com/xxyzz), the chapter 8.17 of APUE mentioned use `times`  to record the CPU time of a process

2. In `test.c` , we can print some useful information such as the page size and the overhead of function `gettimeofday()`

    In macOS X, we can also print some useful system information with the command `$ ulimit -a`

    `pthread_setaffinity_np` does not work on macOS. Refer to this blog, we can customize a similar function to bind our program to one core of CPU.

3. In `test.sh` , the measurement result would be printed to the terminal, whereas, the `data.sh` saves the measurement result into a file named 'data'.

    ```bash
    # measure the tlb access time with the page size from 1 ~ 1024(2^10)
    $ sh test.sh 10

    # maximum number of pages 2^14
    $ sh test.sh 14
    Jump     1 pages | time    0.0 nanoseconds
    Jump     2 pages | time    0.0 nanoseconds
    Jump     4 pages | time    0.0 nanoseconds
    Jump     8 pages | time    0.0 nanoseconds
    Jump    16 pages | time    0.1 nanoseconds
    Jump    32 pages | time    0.3 nanoseconds
    Jump    64 pages | time    0.7 nanoseconds
    Jump   128 pages | time    1.4 nanoseconds
    Jump   256 pages | time    2.7 nanoseconds
    Jump   512 pages | time    5.7 nanoseconds
    Jump  1024 pages | time   11.1 nanoseconds
    Jump  2048 pages | time   19.6 nanoseconds
    Jump  4096 pages | time   37.8 nanoseconds
    Jump  8192 pages | time   81.7 nanoseconds
    ```

 4. For ploting the result, run `python figure.py` the program will automatically draw the figure with data in 'data' file.

![Chapter%2019%20-%20TLBs%20bb1b987310434eba82f98cc126a60f6b/Untitled.png](Chapter%2019%20-%20TLBs%20bb1b987310434eba82f98cc126a60f6b/Untitled.png)
