# Chapter14-Memory API

Q1.First, write a simple program called null.c that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Compile this into an executable called null. What happens when you run this program?

```bash
$ ./null
[1]    29765 segmentation fault (core dumped)  ./null
```

Q2. Next, compile this program with symbol information included(with the -g flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing gdb null and then, once gdb is running, typing run. What does gdb show you?

```bash
$ gcc -o -g null null.c

Program received signal SIGSEGV, Segmentation fault.
0x000055555555465e in main () at null.c:6
6		printf("%d\n", *ptr);
```

Q3. Finally, use the `valgrind` tool on this program. We’ll use the `memcheck` tool that is a part of `valgrind` to analyze what happens. Run this by typing in the following: `valgrind --leak-check=yes ./null` . What happens when you run this? Can you interpret the output from the tool?

```bash
$ valgrind --tool=memcheck ./null
==30088== Memcheck, a memory error detector
==30088== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==30088== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==30088== Command: ./null
==30088==
==30088== Invalid read of size 4
==30088==    at 0x10865E: main (null.c:6)
==30088==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==30088==
==30088==
==30088== Process terminating with default action of signal 11 (SIGSEGV)
==30088==  Access not within mapped region at address 0x0
==30088==    at 0x10865E: main (null.c:6)
==30088==  If you believe this happened as a result of a stack
==30088==  overflow in your program's main thread (unlikely but
==30088==  possible), you can try to increase the size of the
==30088==  main thread stack using the --main-stacksize= flag.
==30088==  The main thread stack size used in this run was 8388608.
==30088==
==30088== HEAP SUMMARY:
==30088==     in use at exit: 0 bytes in 0 blocks
==30088==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==30088==
==30088== All heap blocks were freed -- no leaks are possible
==30088==
==30088== For counts of detected and suppressed errors, rerun with: -v
==30088== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
[1]    30088 segmentation fault (core dumped)  valgrind --tool=memcheck ./null

# ubuntu @ ip-172-31-47-222 in ~/ostep [21:50:56] C:139
$ cat null.c
#include<stdlib.h>
#include<string.h>

int main(void){
	int * ptr = NULL;
	printf("%d\n", *ptr);
}
```

Q4. Write a simple program that allocates memory using malloc() but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about `valgrind` (again with the `--leak-check=yes` flag)?

- gdb is not able to detect memory leak
- after process exit, OS would reclaim all address space even if there are leak memory on heap

```bash
$ valgrind --tool=memcheck ./null 20
==30449== Memcheck, a memory error detector
==30449== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==30449== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==30449== Command: ./null 20
==30449==
hello there!

==30449==
==30449== HEAP SUMMARY:
==30449==     in use at exit: 20 bytes in 1 blocks
==30449==   total heap usage: 2 allocs, 1 frees, 1,044 bytes allocated
==30449==
==30449== LEAK SUMMARY:
==30449==    definitely lost: 20 bytes in 1 blocks
==30449==    indirectly lost: 0 bytes in 0 blocks
==30449==      possibly lost: 0 bytes in 0 blocks
==30449==    still reachable: 0 bytes in 0 blocks
==30449==         suppressed: 0 bytes in 0 blocks
==30449== Rerun with --leak-check=full to see details of leaked memory
==30449==
==30449== For counts of detected and suppressed errors, rerun with: -v
==30449== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

Q5. Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using `valgrind`? Is the program correct?

```java
ptr[0]: 1
==2424== Invalid write of size 4
==2424==    at 0x10871D: main (in /home/ubuntu/data)
==2424==  Address 0x522f1d0 is 0 bytes after a block of size 400 alloc'd
==2424==    at 0x4C31B0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==2424==    by 0x1086EB: main (in /home/ubuntu/data)
==2424==
==2424== Invalid read of size 4
==2424==    at 0x10872D: main (in /home/ubuntu/data)
==2424==  Address 0x522f1d0 is 0 bytes after a block of size 400 alloc'd
==2424==    at 0x4C31B0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==2424==    by 0x1086EB: main (in /home/ubuntu/data)
==2424==
ptr[100]: 100
```

Q6. Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use `valgrind` on it?

- we can still access the value

```bash
$ ./data
ptr[9]: 99
```

- `valgrind`

```bash
==2500== Invalid read of size 4
==2500==    at 0x108752: main (in /home/ubuntu/data)
==2500==  Address 0x522f064 is 36 bytes inside a block of size 400 free'd
==2500==    at 0x4C32D3B: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==2500==    by 0x108749: main (in /home/ubuntu/data)
==2500==  Block was alloc'd at
==2500==    at 0x4C31B0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==2500==    by 0x10872B: main (in /home/ubuntu/data)
==2500==
```

Q7. Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?

- run time error

```bash
$ ./data
free(): invalid pointer
[1]    2529 abort (core dumped)  ./data
```