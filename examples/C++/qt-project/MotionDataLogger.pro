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

PROJECT_DIR = $${PWD}/../MotionDataLogger
SOURCES += $${PROJECT_DIR}/main.cpp

OTHER_FILES+=$${PROJECT_DIR}/CMakeLists.txt

win32 {
    contains(QT_ARCH,i386) {
        LIBS += -lglut32
    }
    contains(QT_ARCH,x86_64) {
        LIBS += -lglut64
    }
}else:ios {
}else:macx{
}else:unix{
    LIBS += -L/usr/lib/x86_64-linux-gnu/mesa
    LIBS += -L/usr/lib/x86_64-linux-gnu
    LIBS +=  -lGL -lGLU -lglut
}
