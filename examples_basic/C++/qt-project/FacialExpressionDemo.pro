TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(deployment.pri)
qtcAddDeployment()

TARGET = $${TARGET}$${TARGET_POSTFIX}
OBJECTS_DIR = $${EXAMPLES_ROOT}/build/$${TARGET}
MOC_DIR = $$OBJECTS_DIR/.moc
RCC_DIR = $$OBJECTS_DIR/.qrc
UI_DIR = $$OBJECTS_DIR/.u

PROJECT_DIR = $${PWD}/../FacialExpressionDemo

HEADERS += $${PROJECT_DIR}/Socket.h
HEADERS += $${PROJECT_DIR}/FacialExpressionControl.h

SOURCES += $${PROJECT_DIR}/Socket.cpp
SOURCES += $${PROJECT_DIR}/FacialExpressionControl.cpp
SOURCES += $${PROJECT_DIR}/FacialExpressionDemo.cpp

OTHER_FILES+=$${PROJECT_DIR}/CMakeLists.txt
