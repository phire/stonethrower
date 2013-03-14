TEMPLATE = subdirs

CONFIG += ordered
QMAKE_CXXFLAGS += -std=c++11

SUBDIRS = simulation \
      debugGui \
    gameGui

debugGui.depends = simulation
gameGui.depends  = simulation
