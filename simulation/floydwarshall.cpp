#include "floydwarshall.h"
#include <math.h>
#include <xmmintrin.h>

#define SSE

FloydWarshall::FloydWarshall(Graph *graph) : height(Intersection::count),
#ifdef SSE // padd the table width to a multiple of 4
    width((height & 3) == 0 ? height : height + (4 - (height & 3))) {
#else
    width(height) {
#endif

    // Allocate the table outside of the thread, so we can always assume it exsists when we terminate the thread
    table = (float *) _mm_malloc(sizeof(float) * width * height, 16); // aligning it gives us a 40% speed boost

    build(graph);
}

void FloydWarshall::abort() {
    terminate();
}

void FloydWarshall::build(Graph *graph) {
    for(unsigned int i=0; i < width * height; i++) // Init table to infinity
        table[i] = INFINITY;

    for(unsigned int i=0; i < height; i++) // Nodes have zero distance to eachother
        table[i * width + i] = 0.0;

    for(Road *r : graph->edges) { // Plop the edge lengths into the table
        table[r->start->num * width + r->end->num] = r->length;
        table[r->end->num * width + r->start->num] = r->length; // our roads are (currently) bidirectional
    }

#ifdef SSE // This is now memory bound, can't be optimised any futher.
    for(unsigned int k=0; k < height; k++) {
        float *kcol = table + k * width; // stop GCC from being stupid and recalculating these in the inner loop
        for(unsigned int i=0; i < height; i++) {
            float *icol = table + i * width; // GCC
            __m128 ik = _mm_load1_ps(icol + k); // load table[i][k] 4 times
            for(unsigned int j=0; j < width; j+=4) {
                __m128 kj = _mm_load_ps(kcol + j); // load table[k][j to j+3]
                __m128 ij = _mm_load_ps(icol + j); // load table[i][j to j+3]
                __m128 min = _mm_min_ps(_mm_add_ps(ik, kj), ij); // Choose minimums of original value and k+i
                _mm_store_ps(icol + j, min); // Store back to memory
            }
        }
    }

#else
    for(int k=0; k < width; k++)
        for(int i=0; i < width; i++)
            for(int j=0; j < width; j++)
                if(table[i * width + k] + table[k * width + j] < table[i * width + j])
                    table[i * width + j] = table[i * width + k] + table[k * width + j];
#endif
}
