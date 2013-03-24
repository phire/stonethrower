#include <QCoreApplication>

#include "benchmark.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Benchmark benchmark;
    
    QMetaObject::invokeMethod( &benchmark, "run", Qt::QueuedConnection );
    QObject::connect(&benchmark, SIGNAL(done()), &a, SLOT(quit()));

    return a.exec();
}
