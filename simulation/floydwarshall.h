#ifndef FLOYDWARSHALL_H
#define FLOYDWARSHALL_H

#include "graph.h"
#include <QThread>

class FloydWarshall : public QThread
{
public:
    FloydWarshall(Graph *);
    void abort(); // stop generating table, it is now out of date.

    float *table;
    const unsigned int height;
    const unsigned int width;

private:
    void build(Graph *graph);
};

#endif // FLOYDWARSHALL_H
