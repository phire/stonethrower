#include "floydwarshall.h"
#include <math.h>

FloydWarshall::FloydWarshall(Graph *graph) {
    QTime t;
    t.start();
    build(graph);
    qDebug("Floyd-Warshall: built table for %i nodes in %d ms", graph->edges.size(), t.elapsed());
}

void FloydWarshall::build(Graph *graph) {
    int count = Intersection::count;
    table = new float[count * count];

    for(int i=0; i < count * count; i++) // Init table to infinity
        table[i] = INFINITY;

    for(int i=0; i < count; i++)
        table[i*count+i] = 0.0;

    for(Road *r : graph->edges) {
        table[r->start->num * count + r->end->num] = r->length;
        table[r->end->num * count + r->start->num] = r->length;
    }

    for(int k=0; k < count; k++)
        for(int i=0; i < count; i++)
            for(int j=0; j < count; j++)
                if(table[i * count + k] + table[k * count + j] < table[i * count + j])
                    table[i * count + j] = table[i * count + k] + table[k * count + j];
}
