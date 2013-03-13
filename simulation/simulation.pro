TARGET = simulation

TEMPLATE = lib
CONFIG += staticlib
DEFINES = SIMULATION_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

SOURCES = graph.cpp
HEADERS = graph.h
