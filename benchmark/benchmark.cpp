#include "benchmark.h"
#include <xmmintrin.h>

#include "floydwarshall.h"
#include "graph.h"

Benchmark::Benchmark(QObject *parent) :
    QObject(parent) {}

void Benchmark::run() {
    Graph graph;

    QFile file("save.sto");
    if(!file.exists())
        return;
    file.open(QIODevice::ReadOnly);
    QByteArray ba = qUncompress(file.readAll());
    QDataStream in(ba);
    in >> graph;

    QTime q;
    q.start();
    FloydWarshall t1(&graph);
    FloydWarshall t2(&graph);
    FloydWarshall t3(&graph);
    FloydWarshall t4(&graph);
    FloydWarshall t5(&graph);
    FloydWarshall t6(&graph);
    FloydWarshall t7(&graph);
    FloydWarshall t8(&graph);
    FloydWarshall t9(&graph);
    FloydWarshall t0(&graph);
    qDebug("Benchmarked FloydWarshall @ %d ms - %d nodes, %d edges", q.elapsed(), graph.nodes.size(), graph.edges.size());
    _mm_free(t1.table); _mm_free(t2.table); _mm_free(t3.table); _mm_free(t4.table); _mm_free(t5.table);
    _mm_free(t6.table); _mm_free(t7.table); _mm_free(t8.table); _mm_free(t9.table); _mm_free(t0.table);

    emit done();
}
