#include <QGuiApplication>
#include "graphwindow.h"

int main(int argc, char **argv) {
    QGuiApplication a(argc, argv);

    GraphWindow window;
    window.show();

    return a.exec();
}
