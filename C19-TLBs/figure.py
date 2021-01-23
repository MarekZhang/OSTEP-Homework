#!/usr/bin/python
import sys
import re
import matplotlib.pyplot as plt
from matplotlib.pyplot import MultipleLocator
import numpy as np
import math

file = open('data', 'r')
nano = []
page = []
str_re = re.compile('\d+\.\d+')
Lines = file.readlines()
for line in Lines:
    for s in line.split():
        match = re.search(str_re, s)
        if match:
            nano.append(s)
        if s.isdigit():        
            page.append(s)

a = np.arange(len(page))
x = 2**a
nano = [float(s) for s in nano]

page = np.array(page)

plt.plot(a, nano, marker='o', color='blue')
x = [math.log2(int(e)) for e in page]
print(page)
print(nano)
y_major_locator=MultipleLocator(2.0)
ax=plt.gca()
ax.yaxis.set_major_locator(y_major_locator)
plt.ylim(0,25)
plt.plot(page, nano, marker='o', color='blue')
plt.xlabel('Number of pages')
plt.ylabel('Time per Access(ns)')
plt.xticks(x, page)
plt.title("TLB size measurement (macOS X)")
plt.savefig('tlb.png', dpi=227)
plt.show()
