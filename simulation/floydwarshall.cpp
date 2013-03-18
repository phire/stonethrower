#include "floydwarshall.h"
#include <math.h>
#define SSE
#include <xmmintrin.h>


FloydWarshall::FloydWarshall(Graph *graph) : height(Intersection::count),
#ifdef SSE
    width((height & 3) == 0 ? height : height + (4 - (height & 3))) {
#else
    width(height) {
#endif

    // Allocate the table outside of the thread, so we can always assume it exsists when we terminate the thread
    table = new float[width * height];

    QTime t;
    t.start();
    build(graph);
    qDebug("Floyd-Warshall: built table for %i nodes in %d ms", graph->edges.size(), t.elapsed());
}

void FloydWarshall::abort() {
    terminate();
}

void FloydWarshall::build(Graph *graph) {
    for(unsigned int i=0; i < width * height; i++) // Init table to infinity
        table[i] = INFINITY;

    for(unsigned int i=0; i < height; i++)
        table[i * width + i] = 0.0;

    for(Road *r : graph->edges) {
        table[r->start->num * width + r->end->num] = r->length;
        table[r->end->num * width + r->start->num] = r->length;
    }

#ifdef SSE
    for(unsigned int k=0; k < height; k++) {
        float *kcol = table + k * width; // stop GCC from being stupid and recalculating these in the inner loop
        for(unsigned int i=0; i < height; i++) {
            float *icol = table + i * width; // GCC
            __m128 ik = _mm_load1_ps(icol + k);
            for(unsigned int j=0; j < width; j+=4) {
                __m128 kj = _mm_loadu_ps(kcol + j);
                __m128 ij = _mm_loadu_ps(icol + j);
                __m128 min = _mm_min_ps(_mm_add_ps(ik, kj), ij);
                _mm_storeu_ps(icol + j, min);
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
