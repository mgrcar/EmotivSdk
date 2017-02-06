TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(deployment.pri)
qtcAddDeployment()

TARGET = $${TARGET}$${TARGET_POSTFIX}
OBJECTS_DIR = $${EXAMPLES_ROOT}/build_$${QT_ARCH}/$${TARGET}
MOC_DIR = $$OBJECTS_DIR/.moc
RCC_DIR = $$OBJECTS_DIR/.qrc
UI_DIR = $$OBJECTS_DIR/.u

PROJECT_DIR = $${PWD}/../EmoStateLogger
SOURCES += $${PROJECT_DIR}/main.cpp

OTHER_FILES+=$${PROJECT_DIR}/CMakeLists.txt

message ("LIB $${LIBS}")
message ("DESTDIR $${DESTDIR}")
message ("QT_ARCH $${QT_ARCH}")
