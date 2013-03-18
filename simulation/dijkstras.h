#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#define GHEAP_CPP11
#include "gheap/gheap_cpp11.hpp"
#include "gheap/gpriority_queue.hpp"

#include "graph.h"

class Dijkstras
{
public:
    Dijkstras(Graph *);

    float *table;

private:
    void build(Graph *graph);
};

#endif // DIJKSTRAS_H
