
TEMPLATE = app

TARGET = stonethrower
DESTDIR = ../

INCLUDEPATH += ../simulation
LIBS += ../simulation/libsimulation.a
PRE_TARGETDEPS += ../simulation/libsimulation.a

SOURCES =   main.cpp \
            graphwindow.cpp

HEADERS +=  graphwindow.h
