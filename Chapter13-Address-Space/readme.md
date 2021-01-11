# Chapter13-Address-Space

Q4. Now, while running your memory-user program, also (in a different terminal window, but on the same machine) run the free tool. How do the memory usage totals change when your program is running? How about when you kill the memory-user program? Do the numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?

```bash
➜  ~ free -m
              total        used        free      shared  buff/cache   available
Mem:           1988         319         683         420         985        1063
Swap:          1023           0        1023
➜  ~ free -m
              total        used        free      shared  buff/cache   available
Mem:           1988         509         488         420         990         873
Swap:          1023           0        1023
➜  ~ ps a
  PID TTY      STAT   TIME COMMAND
   21 pts/0    Ss     0:00 -zsh
  115 pts/1    Ss     0:00 -zsh
  214 pts/0    S+     0:03 ./memory 200000000 20
  218 pts/1    R+     0:00 ps a
➜  ~ kill 214
[1]    214 terminated  ./memory 200000000 20
```

Q5~8

```bash
➜  ~ ps auxw
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.2  65512  5184 ?        Ss   16:54   0:00 /usr/sbin/sshd -D
root         8  4.2  0.3  90480  6592 ?        Ss   16:54   1:55 sshd: root@pts/0
root        21  0.0  0.2  50528  6032 pts/0    Ss   16:54   0:00 -zsh
root       102  0.0  0.3  90480  6480 ?        Rs   17:15   0:00 sshd: root@pts/1
root       115  0.0  0.2  47396  4872 pts/1    Ss   17:16   0:00 -zsh
root       425 59.6  9.6 199668 196544 pts/0   R+   17:40   0:01 ./memory 200000000 3000
root       426  0.0  0.1  34424  2840 pts/1    R+   17:40   0:00 ps auxw
➜  ~ pmap 425 -X
425:   ./memory 200000000 3000
         Address Perm   Offset Device   Inode   Size    Rss    Pss Referenced Anonymous LazyFree ShmemPmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
        00400000 r-xp 00000000  fe:01 3809071      4      4      4          4         0        0              0              0               0    0       0      0           0 memory
        00600000 r--p 00000000  fe:01 3809071      4      4      4          4         4        0              0              0               0    0       0      0           0 memory
        00601000 rw-p 00001000  fe:01 3809071      4      4      4          4         4        0              0              0               0    0       0      0           0 memory
        01cce000 rw-p 00000000  00:00       0    132      4      4          4         4        0              0              0               0    0       0      0           0 [heap]
    7f5ecd065000 rw-p 00000000  00:00       0 195316 195316 195316     195316    195316        0              0              0               0    0       0      0           0
    7f5ed8f22000 r-xp 00000000  fe:01 3673533   1792   1004    153       1004         0        0              0              0               0    0       0      0           0 libc-2.23.so
    7f5ed90e2000 ---p 001c0000  fe:01 3673533   2048      0      0          0         0        0              0              0               0    0       0      0           0 libc-2.23.so
    7f5ed92e2000 r--p 001c0000  fe:01 3673533     16     16     16         16        16        0              0              0               0    0       0      0           0 libc-2.23.so
    7f5ed92e6000 rw-p 001c4000  fe:01 3673533      8      8      8          8         8        0              0              0               0    0       0      0           0 libc-2.23.so
    7f5ed92e8000 rw-p 00000000  00:00       0     16      8      8          8         8        0              0              0               0    0       0      0           0
    7f5ed92ec000 r-xp 00000000  fe:01 3673526    152    144     20        144         0        0              0              0               0    0       0      0           0 ld-2.23.so
    7f5ed9507000 rw-p 00000000  00:00       0     12     12     12         12        12        0              0              0               0    0       0      0           0
    7f5ed9511000 r--p 00025000  fe:01 3673526      4      4      4          4         4        0              0              0               0    0       0      0           0 ld-2.23.so
    7f5ed9512000 rw-p 00026000  fe:01 3673526      4      4      4          4         4        0              0              0               0    0       0      0           0 ld-2.23.so
    7f5ed9513000 rw-p 00000000  00:00       0      4      4      4          4         4        0              0              0               0    0       0      0           0
    7ffe4c55d000 rw-p 00000000  00:00       0    132      8      8          8         8        0              0              0               0    0       0      0           0 [stack]
    7ffe4c5bf000 r--p 00000000  00:00       0     12      0      0          0         0        0              0              0               0    0       0      0           0 [vvar]
    7ffe4c5c2000 r-xp 00000000  00:00       0      8      4      0          4         0        0              0              0               0    0       0      0           0 [vdso]
ffffffffff600000 r-xp 00000000  00:00       0      4      0      0          0         0        0              0              0               0    0       0      0           0 [vsyscall]
                                              ====== ====== ====== ========== ========= ======== ============== ============== =============== ==== ======= ====== ===========
                                              199672 196548 195569     196548    195392        0              0              0               0    0       0      0           0 KB
```