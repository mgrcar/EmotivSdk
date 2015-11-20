TEMPLATE = subdirs
#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

#EXAMPLES_ROOT=$${PWD}/../../..
#INCLUDEPATH += $${EXAMPLES_ROOT}/include
#LIBS+=-L$${EXAMPLES_ROOT}/bin -L$${EXAMPLES_ROOT}/lib

#message ("INCLUDEPATH $$INCLUDEPATH $$LIBS")
#include(deployment.pri)
#qtcAddDeployment()

SUBDIRS += EEGLogger.pro
SUBDIRS += MultiDongleEEGLogger.pro
SUBDIRS += MultilChannelEEGLogger.pro

