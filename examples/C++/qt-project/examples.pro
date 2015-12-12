TEMPLATE = subdirs
#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt

#EXAMPLES_ROOT=$${PWD}/../../..
#INCLUDEPATH += $${EXAMPLES_ROOT}/include
#LIBS+=-L$${EXAMPLES_ROOT}/bin -L$${EXAMPLES_ROOT}/lib



#SOURCES += $${PWD}/../EmoStateLogger/main.cpp
#SOURCES += $${PWD}/../EmoStateLogger/main.cpp

#message ("INCLUDEPATH $$INCLUDEPATH $$LIBS")
#include(deployment.pri)
#qtcAddDeployment()

SUBDIRS += AverageBandPowers.pro
SUBDIRS += EmoStateLogger.pro
SUBDIRS += FacialExpressionDemo.pro
SUBDIRS += GyroData.pro
SUBDIRS += HeadsetInformationLogger.pro
SUBDIRS += MentalCommandDemo.pro
SUBDIRS += MotionDataLogger.pro
SUBDIRS += MultiDongleConnection.pro
SUBDIRS += SavingAndLoadingProfileCloud.pro

