#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QObject>

class Benchmark : public QObject
{
    Q_OBJECT
public:
    explicit Benchmark(QObject *parent = 0);
    
signals:
    void done();
    
public slots:
    void run();
};

#endif // BENCHMARK_H
