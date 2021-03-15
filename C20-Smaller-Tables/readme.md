# Chapter 20 - Paging: Smaller Tables

Q1. With a linear page table, you need a single register to locate the page table, assuming that hardware does the lookup upon a TLB miss. How many registers do you need to locate a two-level page table? A three-level table?

- two, three

Q2. Use the simulator to perform translations given random seeds 0, 1, and 2, and check your answers using the -c flag. How many memory references are needed to perform each lookup?

- virtual address: 0x6c74 → ~~0~~1(valid)1011 (page directory index)| 00011(page table offset) | 10100 → five upper bits represents page directory index 11011 = 27  → base register = 17, 27th byte: a0 → 1(valid)0100000 → page 32, offset 00011 = 3 → 0xe1 → page 97, offset 20 → value 06

Q3. Given your understanding of how cache memory works, how do you think memory references to the page table will behave in the cache? Will they lead to lots of cache hits (and thus fast accesses?) Or lots of misses (and thus slow accesses)?

- cause slow accesses
