#ifndef FLOYDWARSHALL_H
#define FLOYDWARSHALL_H

#include "graph.h"

class FloydWarshall
{
public:
    FloydWarshall(Graph *);

    float *table;

private:
    void build(Graph *graph);
};

#endif // FLOYDWARSHALL_H
