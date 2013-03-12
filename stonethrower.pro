TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = simulation \
      debugGui \
    gameGui

debugGui.depends = simulation
