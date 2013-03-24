TARGET = simulation

DESTDIR = ./
TEMPLATE = lib
CONFIG += staticlib
DEFINES = SIMULATION_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

SOURCES = graph.cpp \
    savefile.cpp \
    dijkstras.cpp \
    floydwarshall.cpp
HEADERS = graph.h \
    savefile.h \
    gheap/gpriority_queue.hpp \
    gheap/gheap_cpp11.hpp \
    dijkstras.h \
    floydwarshall.h
