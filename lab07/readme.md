# ex1

## scenario 1:

address between each step is 32 byte, equals to the cache size, therefore
they map to the same block, and each time we write we need to eject the block and get from memory, led to a hit rate of zero.

## scenario 2:

this time due to option 1, we have 2 memory access at one loop, because the block size is 16, we load 4 int into cache one time, also fit next access, hit rate is 3 out of 4.

## scenario 3:

same as 1, 8 byte block and 1 step size led to hit rate 1 out of 2.

# ex2

## best jki,kji

at this two, index of C and A increase at the step of 1, which use the memory locality best.

## worst ikj,kij

each step increse j led to index increase n, therefore 
