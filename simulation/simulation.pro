TARGET = simulation

TEMPLATE = lib
CONFIG += staticlib
DEFINES = SIMULATION_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

SOURCES = graph.cpp \
    savefile.cpp
HEADERS = graph.h \
    savefile.h
