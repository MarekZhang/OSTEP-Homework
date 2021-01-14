# Chapter 16 - Segmentation

Q1. First let’s use a tiny address space to translate some addresses. Here’s a simple set of parameters with a few different random seeds; can you translate the addresses?

![Chapter%2016%20-%20Segmentation%20cc668c27e7ed4e92a9150f76f1e7df20/IMG_667BFA98EF41-1.jpeg](Chapter%2016%20-%20Segmentation%20cc668c27e7ed4e92a9150f76f1e7df20/IMG_667BFA98EF41-1.jpeg)

```bash
./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0
./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1
./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2
```

Q2. Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). What is the highest legal virtual address in segment 0? What about the lowest legal virtual address in segment 1? What are the lowest and highest illegal addresses in this entire address space? Finally, how would you run `segmentation.py` with the -A flag to test if you are right?

1. seg 0 valid virtual address 0~19
2. seg 1 valid virtual address 108 ~ 127
3. lowest illegal address 20
4. highest illegal address 107

```bash
$ ./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 -A 19,108,20,107 -c
```

Q3. Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: valid, valid, violation, ..., violation, valid, valid? Assume the following parameters:

```bash
$ ./segmentation.py -a 16 -p 128 --b0 0 --l0 2 --b1 127 --l1 2 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 -c
```

Q4. Assume we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (not segmentation violations). How should you configure the simulator to do so? Which parameters are important to getting this outcome?

```bash
$ ./segmentation.py -a 16 -p 128 --b0 0 --l0 7 --b1 15 --l1 7 -s 1 -c
// valid memory size equals to 90% of the whole virtual memory size
```

Q5. Can you run the simulator such that no virtual addresses are valid? How?

```bash
$ ./segmentation.py -a 16 -p 128 --b0 0 --l0 0 --b1 15 --l1 0 -s 1 -c
```