#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T02:14:04
#
#-------------------------------------------------

#QT       += core

#QT       -= gui

TARGET = benchmark
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += ../simulation
LIBS += ../simulation/libsimulation.a


SOURCES += main.cpp \
    benchmark.cpp

HEADERS += \
    benchmark.h
