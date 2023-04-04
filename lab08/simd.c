#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        int *arr=vals;
        __m128i sum_vec = _mm_setzero_si128();
        for(int i=0;i<NUM_ELEMS/4;i++,arr+=4){
            __m128i tmp= _mm_loadu_si128((__m128i *) arr);
            tmp = _mm_and_si128(tmp, _mm_cmpgt_epi32(tmp,_127));
            sum_vec =  _mm_add_epi32(tmp,sum_vec);
        }
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
        int vec[4];
        _mm_storeu_si128((__m128i *) vec, sum_vec);
        result+=vec[0]+vec[1]+vec[2]+vec[3];
        /* Hint: you'll need a tail case. */
    }
    
    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        /* Copy your sum_simd() implementation here, and unroll it */
        int *arr=vals;
        __m128i sum_vec = _mm_setzero_si128();
        __m128i sum_vec2 = _mm_setzero_si128();
        __m128i sum_vec3 = _mm_setzero_si128();
        __m128i sum_vec4 = _mm_setzero_si128();
        __m128i tmp1,tmp2,tmp3,tmp4;
        for(int i=0;i<NUM_ELEMS/16;i++,arr+=16){
            tmp1= _mm_loadu_si128((__m128i *) arr);
            tmp2= _mm_loadu_si128((__m128i *) (arr+4));
            tmp3= _mm_loadu_si128((__m128i *) (arr+8));
            tmp4= _mm_loadu_si128((__m128i *) (arr+12));
            tmp1 = _mm_and_si128(tmp1, _mm_cmpgt_epi32(tmp1,_127));
            tmp2 = _mm_and_si128(tmp2, _mm_cmpgt_epi32(tmp2,_127));
            tmp3 = _mm_and_si128(tmp3, _mm_cmpgt_epi32(tmp3,_127));
            tmp4 = _mm_and_si128(tmp4, _mm_cmpgt_epi32(tmp4,_127));
            sum_vec =  _mm_add_epi32(tmp1,sum_vec);
            sum_vec2 =  _mm_add_epi32(tmp2,sum_vec2);
            sum_vec3 =  _mm_add_epi32(tmp3,sum_vec3);
            sum_vec4 =  _mm_add_epi32(tmp4,sum_vec4);
        }
        sum_vec =  _mm_add_epi32(sum_vec2,sum_vec);
        sum_vec =  _mm_add_epi32(sum_vec3,sum_vec);
        sum_vec =  _mm_add_epi32(sum_vec4,sum_vec);
        for(unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
        int vec[4];
        _mm_storeu_si128((__m128i *) vec, sum_vec);
        result+=vec[0]+vec[1]+vec[2]+vec[3];
        /* Hint: you'll need 1 or maybe 2 tail cases here. */
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
