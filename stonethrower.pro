TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = simulation \
		  debugGui

debugGui.depends = simulation
