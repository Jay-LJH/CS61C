#include "transpose.h"
/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src)
{
    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src)
{
    // YOUR CODE HERE
    int time = n / blocksize + ((n % blocksize) ? 1 : 0);
    for (int i = 0; i < time; i++)
    {
        for (int j = 0; j < time; j++)
        {
            for (int x = 0; x < blocksize; x++)
            {
                if ((x + i * blocksize) >= n)
                    break;
                for (int y = 0; y < blocksize; y++)
                {
                    if ((y + j * blocksize) >= n)
                        break;
                    dst[j * blocksize + y + (x + i * blocksize) * n] =
                        src[(x + i * blocksize) + (y + j * blocksize) * n];
                }
            }
        }
    }
}
